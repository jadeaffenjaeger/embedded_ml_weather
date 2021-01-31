#ifndef DPS_310_H
#define DPS_310_H

#include <stdint.h>

#include "dps310_registers.h"

#define DPS310_READ_WAIT_FOR_REG_ATTEMPTS 3

#define DPS310_I2C_MAX_BUFF_SIZE 10
#define I2C_DPS310_ADDRESS 0x77

typedef enum {
    IDLE = DPS310_MEAS_CFG_MEAS_CTRL_IDLE,
    PRESSURE_MEASUREMENT = DPS310_MEAS_CFG_MEAS_CTRL_PRS,
    TEMPERATURE_MEASUREMENT = DPS310_MEAS_CFG_MEAS_CTRL_TMP
} dps310_mode_t;

int16_t dps310_init();

int16_t dps310_configure_temperature(uint8_t data);

int16_t dps310_configure_pressure(uint8_t data);

int16_t dps310_read(float *p_temperature, float *p_pressure);

int16_t dps310_wakeup(dps310_mode_t mode);

int16_t dps310_sleep();

int16_t dps310_reset();

#endif // DPS_310_H