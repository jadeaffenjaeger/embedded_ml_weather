#ifndef DRIVER_I2C_H
#define DRIVER_I2C_H

#include "stdint.h"

void i2c_init(void);

int8_t dps310_i2c_read(uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);
int8_t dps310_i2c_write(uint8_t address, uint8_t reg, const uint8_t *data, uint16_t count);

int8_t bme280_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
int8_t bme280_i2c_write(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);

void dps310_i2c_delay_ms(uint32_t delay);
void bme280_delay_us(uint32_t period, void *intf_ptr);

#endif