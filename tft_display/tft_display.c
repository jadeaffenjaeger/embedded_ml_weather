#include <time.h>
#include <stdio.h>

#include "tft_display/tft_display.h"

#include "GUI.h"
#include "mtb_st7789v.h"
#include "cy8ckit_028_tft_pins.h"

// Weather Icon Bitmaps
#include "bitmaps/sun.h"
#include "bitmaps/rain.h"
#include "bitmaps/cloud.h"

const mtb_st7789v_pins_t tft_pins =
{
    .db08 = CY8CKIT_028_TFT_PIN_DISPLAY_DB8,
    .db09 = CY8CKIT_028_TFT_PIN_DISPLAY_DB9,
    .db10 = CY8CKIT_028_TFT_PIN_DISPLAY_DB10,
    .db11 = CY8CKIT_028_TFT_PIN_DISPLAY_DB11,
    .db12 = CY8CKIT_028_TFT_PIN_DISPLAY_DB12,
    .db13 = CY8CKIT_028_TFT_PIN_DISPLAY_DB13,
    .db14 = CY8CKIT_028_TFT_PIN_DISPLAY_DB14,
    .db15 = CY8CKIT_028_TFT_PIN_DISPLAY_DB15,
    .nrd  = CY8CKIT_028_TFT_PIN_DISPLAY_NRD,
    .nwr  = CY8CKIT_028_TFT_PIN_DISPLAY_NWR,
    .dc   = CY8CKIT_028_TFT_PIN_DISPLAY_DC,
    .rst  = CY8CKIT_028_TFT_PIN_DISPLAY_RST
};

static void _disp_title(const char *s) {
    GUI_SetFont(&GUI_Font16_ASCII);
    GUI_DispString(s);
    GUI_DispNextLine();
}

static void _disp_value(float val, uint8_t decimals) {
    GUI_SetFont(&GUI_Font32B_ASCII);
    GUI_DispFloat(val, decimals);
    GUI_DispNextLine();
}

static void _disp_time(struct tm *date_time) {
    GUI_SetFont(&GUI_Font16B_ASCII);
    char buffer [80];
    strftime(buffer, 80, "%A %m/%d/%Y %T", date_time);

    GUI_GotoXY(160, 50);
    GUI_DispStringHCenterAt(buffer, 160, 220);
}

void display_init(void) {
    cy_rslt_t result;
    result = mtb_st7789v_init8(&tft_pins);
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    GUI_Init();
    GUI_SetBkColor(GUI_WHITE);
    GUI_SetColor(GUI_GRAY_9A);
    GUI_Clear();

    GUI_SetPenSize(2);
    GUI_DrawLine(10, 20, 310, 20);
    GUI_DrawLine(10, 215, 310, 215);
    GUI_DrawLine(160, 25, 160, 210);

    GUI_SetColor(GUI_BLACK);

    GUI_SetFont(&GUI_Font16B_ASCII);
    GUI_DispStringHCenterAt("tinyAI Weather Forecast", 160, 3);

    GUI_SetLBorder(180);
}

void display_update(measurement_t *measurement, struct tm *date_time) {
    GUI_GotoXY(180, 50);

    _disp_title("Temperature [C]");
    _disp_value(measurement->temperature, 5);

    _disp_title("Pressure [hPa]");
    _disp_value(measurement->pressure, 8);

    _disp_title("Humidity [%]");
    _disp_value(measurement->humidity, 5);

    _disp_time(date_time);
}

void display_prediction(predictions_t *predictions) {
    GUI_SetFont(&GUI_Font32B_ASCII);

    static weather_class_t last_pred = INVALID;

    if (predictions->class_type != last_pred)
    {
        GUI_RECT Rect = {15, 162, 143, 198};
        GUI_ClearRect(15, 162, 143, 190);
        GUI_DispStringInRect(predictions->class_name, &Rect, GUI_TA_HCENTER);

        switch (predictions->class_type)
        {
        case CLOUDS:
            GUI_BMP_Draw(cloud_bmp, 15, 30);
            break;
        case RAIN:
            GUI_BMP_Draw(rain_bmp, 15, 30);
            break;
        case CLEAR:
            GUI_BMP_Draw(sun_bmp, 15, 30);
            break;
        default:
            break;
        }
        last_pred = predictions->class_type;
    }

    const uint8_t BUF_LEN = 80;
    char buffer[BUF_LEN];
    snprintf(buffer, BUF_LEN, "Confidence: %3.1f", predictions->class_score * 100);

    GUI_SetFont(&GUI_Font16_ASCII);
    GUI_DispStringHCenterAt(buffer, 79, 192);
}