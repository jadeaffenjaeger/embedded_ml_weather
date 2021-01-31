#include "measure.h"

#include "dps310/dps310.h"
#include "bme280/bme280.h"
#include "driver/i2c.h"

struct bme280_dev bme280_dev;
struct bme280_data bme280_comp_data;
uint8_t bme280_addr = BME280_I2C_ADDR_PRIM;

const float PA_TO_HPA = 0.01;

int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev)
{
    int8_t rslt;
    uint8_t settings_sel;

    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_1X;
    dev->settings.osr_t = BME280_OVERSAMPLING_1X;
    dev->settings.filter = BME280_FILTER_COEFF_OFF;

    settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL;

    rslt = bme280_set_sensor_settings(settings_sel, dev);
    rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);
    rslt = bme280_get_sensor_data(BME280_ALL, &bme280_comp_data, dev);

    return rslt;
}

void sensors_init(void) {
    bme280_dev.intf_ptr = &bme280_addr;
    bme280_dev.intf = BME280_I2C_INTF;
    bme280_dev.read = bme280_i2c_read;
    bme280_dev.write = bme280_i2c_write;
    bme280_dev.delay_us = bme280_delay_us;

    i2c_init();
    dps310_init();
    bme280_init(&bme280_dev);
}

void sensors_read(measurement_t *measurement) {
    dps310_read(&measurement->temperature, &measurement->pressure);
    stream_sensor_data_forced_mode(&bme280_dev);
    measurement->humidity = bme280_comp_data.humidity;
    measurement->pressure *= PA_TO_HPA;
}

void sensors_normalize(measurement_t *measurement, const normalization_t *norm_factors) {
    measurement->temperature = (measurement->temperature - norm_factors->temperature_mean) / norm_factors->temperature_std;
    measurement->pressure = (measurement->pressure / norm_factors->pressure_mean) / norm_factors->pressure_std;
    measurement->humidity = (measurement->humidity - norm_factors->humidity_mean) / norm_factors->humidity_std;
}