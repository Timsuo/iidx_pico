/*
 * UC1701X LCD controller driver for JLX12864G-086-PC.
 *
 * The manufacturer documents UC1701X as ST7565R-compatible. This driver uses
 * only the LCD controller interface; it does not access the JLX-GB2312 ROM or
 * control the module backlight.
 */

#include "uc1701x.h"

#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"

#include "board_defs.h"

#define UC1701X_SPI_HZ 4000000
#define UC1701X_DEFAULT_RESISTOR_RATIO 0x23
#define UC1701X_DEFAULT_CONTRAST 0x28
#define UC1701X_COLUMN_OFFSET 0

static void write_byte(uint8_t value, bool data)
{
    gpio_put(LCD_DC_PIN, data);
    gpio_put(LCD_CS_PIN, 0);
    spi_write_blocking(LCD_SPI, &value, 1);
    gpio_put(LCD_CS_PIN, 1);
}

void uc1701x_write_command(uint8_t command)
{
    write_byte(command, false);
}

void uc1701x_write_data(const uint8_t *data, size_t length)
{
    if ((data == NULL) || (length == 0)) {
        return;
    }

    gpio_put(LCD_DC_PIN, 1);
    gpio_put(LCD_CS_PIN, 0);
    spi_write_blocking(LCD_SPI, data, length);
    gpio_put(LCD_CS_PIN, 1);
}

void uc1701x_reset(void)
{
    gpio_put(LCD_RST_PIN, 0);
    sleep_ms(100);
    gpio_put(LCD_RST_PIN, 1);
    sleep_ms(100);
}

void uc1701x_set_address(uint8_t page, uint8_t column)
{
    if ((page >= UC1701X_PAGE_COUNT) || (column >= UC1701X_WIDTH)) {
        return;
    }

    uint8_t controller_column = column + UC1701X_COLUMN_OFFSET;
    uc1701x_write_command(0xb0 | page);
    uc1701x_write_command(0x10 | ((controller_column >> 4) & 0x0f));
    uc1701x_write_command(controller_column & 0x0f);
}

void uc1701x_write_page(uint8_t page, const uint8_t *data, size_t length)
{
    if ((page >= UC1701X_PAGE_COUNT) || (data == NULL) || (length == 0)) {
        return;
    }
    if (length > UC1701X_WIDTH) {
        length = UC1701X_WIDTH;
    }

    uc1701x_set_address(page, 0);
    uc1701x_write_data(data, length);
}

void uc1701x_init(void)
{
    gpio_init(LCD_CS_PIN);
    gpio_init(LCD_RST_PIN);
    gpio_init(LCD_DC_PIN);
    gpio_set_dir(LCD_CS_PIN, GPIO_OUT);
    gpio_set_dir(LCD_RST_PIN, GPIO_OUT);
    gpio_set_dir(LCD_DC_PIN, GPIO_OUT);
    gpio_put(LCD_CS_PIN, 1);
    gpio_put(LCD_RST_PIN, 1);
    gpio_put(LCD_DC_PIN, 0);

    spi_init(LCD_SPI, UC1701X_SPI_HZ);
    spi_set_format(LCD_SPI, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    gpio_set_function(LCD_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(LCD_MOSI_PIN, GPIO_FUNC_SPI);

    uc1701x_reset();

    uc1701x_write_command(0xe2);
    sleep_ms(5);
    uc1701x_write_command(0x2c);
    sleep_ms(50);
    uc1701x_write_command(0x2e);
    sleep_ms(50);
    uc1701x_write_command(0x2f);
    sleep_ms(5);
    uc1701x_write_command(UC1701X_DEFAULT_RESISTOR_RATIO);
    uc1701x_write_command(0x81);
    uc1701x_write_command(UC1701X_DEFAULT_CONTRAST);
    uc1701x_write_command(0xa2);
    uc1701x_write_command(0xc8);
    uc1701x_write_command(0xa0);
    uc1701x_write_command(0x40);
    uc1701x_write_command(0xaf);
}
