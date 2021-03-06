/*
 * i2c_phys.h
 *
 * Created: 6/8/2015 1:44:22 PM
 *  Author: Cempaka
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef I2C_PHYS_H_
#define I2C_PHYS_H_

#include "aes132_i2c.h"
#include <stdint.h>
#include "atca_status.h"
#include "i2c_bitbang_arduino.h"

// #include "atca_hal.h"
// #include "atca_iface.h"
// #include "atca_status.h"
// #include "stm32l4xx_hal.h"
// #include "hal_i2c_stm32l475.h"

#include <stdlib.h>
//#include <i2c_master.h>

//! I2C clock
#define I2C_CLOCK (400000.0)

//! Use pull-up resistors.
#define I2C_PULLUP

/** \brief number of polling iterations for TWINT bit in TWSR after
 *         creating a Start condition in #i2c_send_start()
 *
 * Adjust this value considering how long it takes to check a status bit
 * in the I2C status register, decrement the timeout counter,
 * compare its value with 0, and branch.
 */
#define I2C_START_TIMEOUT ((uint8_t)250)

/** \brief number of polling iterations for TWINT bit in TWSR after sending
 *         or receiving a byte.
 *
 * Adjust this value considering how long it takes to check a status bit
 * in the I2C status register, decrement the timeout counter,
 * compare its value with 0, branch, and to send or receive one byte.
 */
#define I2C_BYTE_TIMEOUT ((uint8_t)100)

/** \brief number of polling iterations for TWSTO bit in TWSR after
 *         creating a Stop condition in #i2c_send_stop().
 *
 * Adjust this value considering how long it takes to check a status bit
 * in the I2C control register, decrement the timeout counter,
 * compare its value with 0, and branch.
 */
#define I2C_STOP_TIMEOUT ((uint8_t)250)

// error codes for physical hardware dependent module
// Codes in the range 0x00 to 0xF7 are shared between physical interfaces (SWI,
// I2C, SPI).
// Codes in the range 0xF8 to 0xFF are special for the particular interface.
#define I2C_FUNCTION_RETCODE_SUCCESS                                           \
  ((uint8_t)0x00) //!< Communication with device succeeded.
#define I2C_FUNCTION_RETCODE_COMM_FAIL                                         \
  ((uint8_t)0xF0) //!< Communication with device failed.
#define I2C_FUNCTION_RETCODE_TIMEOUT                                           \
  ((uint8_t)0xF1)                                 //!< Communication timed out.
#define I2C_FUNCTION_RETCODE_NACK ((uint8_t)0xF8) //!< I2C nack

// Function prototypes to be implemented in the target i2c_phys.c
void i2c_enable_phys(void);
void i2c_disable_phys(void);
uint8_t i2c_select_device_phys(uint8_t device_id);
// void i2c_send_start(void)__attribute__((weak));
// void i2c_send_stop(void)__attribute__((weak));
// ATCA_STATUS i2c_send_bytes(uint8_t count, const uint8_t *data)__attribute__((weak));
// void i2c_receive_byte(uint8_t *data)__attribute__((weak));
// void i2c_receive_bytes(uint8_t count, uint8_t *data)__attribute__((weak));
uint8_t i2c_send_slave_address(uint8_t read);
#endif /* I2C_PHYS_H_ */

#ifdef __cplusplus
}
#endif