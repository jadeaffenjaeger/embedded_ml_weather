#include "cyhal.h"
#include "cybsp.h"

#include "driver/i2c.h"
#include "dps310/dps310_errors.h"

cyhal_i2c_t i2c_master_obj;
const uint32_t I2C_MASTER_FREQUENCY = 400000u;
cyhal_i2c_cfg_t i2c_master_config = {CYHAL_I2C_MODE_MASTER, 0, I2C_MASTER_FREQUENCY};

void i2c_init(void) {
    cy_rslt_t result;
    result = cyhal_i2c_init(&i2c_master_obj, CYBSP_I2C_SDA, CYBSP_I2C_SCL , NULL);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
    
    result = cyhal_i2c_configure(&i2c_master_obj, &i2c_master_config);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
}

int8_t dps310_i2c_read(uint8_t address, uint8_t reg, uint8_t *data, uint16_t count) {
    cy_rslt_t result;
    result = cyhal_i2c_master_mem_read(&i2c_master_obj, address, reg, 1, data, count, 0);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
    return DPS310_OK;
}

int8_t dps310_i2c_write(uint8_t address, uint8_t reg, const uint8_t *data, uint16_t count) {
    cy_rslt_t result;
    result = cyhal_i2c_master_mem_write(&i2c_master_obj, address, reg, count, data, count, 0);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
    return DPS310_OK;
}

int8_t bme280_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */
    cy_rslt_t result;
    uint16_t addr = (uint16_t) *((uint8_t *) intf_ptr);
    result = cyhal_i2c_master_mem_read(&i2c_master_obj, addr, reg_addr, 1, reg_data, len, 0);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }    
    return rslt;
}

int8_t bme280_i2c_write(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */
    cy_rslt_t result;
    uint16_t addr = (uint16_t) *((uint8_t *) intf_ptr);
    result = cyhal_i2c_master_mem_write(&i2c_master_obj, addr, reg_addr, len, reg_data, len, 0);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
    return rslt;
}

void dps310_i2c_delay_ms(uint32_t delay) {
    cyhal_system_delay_ms(delay);
}

void bme280_delay_us(uint32_t period, void *intf_ptr)
{
    cyhal_system_delay_us(period);
}