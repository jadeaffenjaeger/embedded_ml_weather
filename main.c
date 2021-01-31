#include <stdlib.h>

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

#include "tft_display/tft_display.h"
#include "measure.h"
#include "driver/rtc.h"

#include "tensorflow/lite/micro/examples/hello_world/main_functions.h"
#include "debug_log_callback.h"

const uint8_t UART_RX_BUF_LEN = 11;
char uart_rx_buf[11];

void debug_log_printf(const char* s)
{
    printf(s);
}     

void uart_event_handler(void *handler_arg, cyhal_uart_event_t event) {
    (void) handler_arg;
    uint64_t unixtime = strtol(uart_rx_buf, NULL, 10);
    rtc_update(unixtime);
    cyhal_uart_read_async(&cy_retarget_io_uart_obj, uart_rx_buf, UART_RX_BUF_LEN-1);
}

int main(void)
{
    cy_rslt_t result;

    measurement_t measurement;
    predictions_t predictions;

    // Per-feature mean and std and from training set
    const normalization_t norm_factors = {
        .temperature_mean = 11.44539464,
        .temperature_std = 9.33416979,
        .humidity_mean = 70.6470914,
        .humidity_std = 21.36824389,
        .pressure_mean = 1019.54981049,
        .pressure_std = 10.3000846
    };

    struct tm date_time;

    result = cybsp_init();
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    __enable_irq();

    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    // Debug logging for TFlite micro
    RegisterDebugLogCallback(debug_log_printf);

    // Initialize peripherals and sensors
    display_init();
    rtc_init();
    sensors_init();
    nn_init();

    // Setup UART buffer for setting the RTC time externally
    uart_rx_buf[UART_RX_BUF_LEN-1] = '\0';
    cyhal_uart_register_callback(&cy_retarget_io_uart_obj, uart_event_handler, NULL);
    cyhal_uart_enable_event(&cy_retarget_io_uart_obj,  CYHAL_UART_IRQ_RX_DONE, 7, true);
    cyhal_uart_read_async(&cy_retarget_io_uart_obj, uart_rx_buf, UART_RX_BUF_LEN-1);

    for (;;)
    {
        sensors_read(&measurement);
        rtc_read(&date_time);
        display_update(&measurement, &date_time);

        // Normalize sensor readings to mean=0 and std=1 for inference
        sensors_normalize(&measurement, &norm_factors);

        // Get class scores
        nn_inference(&measurement, &predictions);

        // Send predicted class to display
        display_prediction(&predictions);

        // Give sensors some time to cool down again
        cyhal_system_delay_ms(5000);
    }
}