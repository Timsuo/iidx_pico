/*
 * UC1701X LCD controller driver for JLX12864G-086-PC.
 */

#ifndef UC1701X_H
#define UC1701X_H

#include <stddef.h>
#include <stdint.h>

#define UC1701X_WIDTH 128
#define UC1701X_PAGE_COUNT 8

void uc1701x_init(void);
void uc1701x_reset(void);

void uc1701x_write_command(uint8_t command);
void uc1701x_write_data(const uint8_t *data, size_t length);

void uc1701x_set_address(uint8_t page, uint8_t column);
void uc1701x_write_page(uint8_t page, const uint8_t *data, size_t length);

#endif
