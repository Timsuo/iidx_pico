/*
 * IIDX Controller Board Definitions
 * WHowe <github.com/whowechina>
 */

#if defined BOARD_IIDX_PICO
#define BOARD_HAS_HALL_KEYS 1
#define BOARD_HAS_SECONDARY_TT_I2C 1
#define BOARD_HAS_ALT_TT_RGB 1
#define BOARD_HAS_LCD 0

/* List of button pins */
#define BUTTON_DEF { 8, 7, 6, 5, 4, 3, 2, 12, 11, 10, 9, 1, 0 }

#define BUTTON_RGB_PIN 13
#define BUTTON_RGB_ORDER GRB // or RGB

#define BUTTON_RGB_NUM 11
#define BUTTON_RGB_MAP { 6, 0, 5, 1, 4, 2, 3, 7, 8, 9, 10}

#define TT_RGB_PIN 28
#define TT_RGB_PIN_2 22
#define TT_RGB_ORDER GRB // or RGB

#define TT_SENSOR_SCL 27
#define TT_SENSOR_SDA 26
#define TT_SENSOR_I2C i2c1

#define TT_SENSOR_SCL_2 17
#define TT_SENSOR_SDA_2 16
#define TT_SENSOR_I2C_2 i2c0

#define TT_SENSOR_I2C_FREQ 377 * 1000

#define HALL_MUX_EN 18
#define HALL_MUX_A0 21
#define HALL_MUX_A1 20
#define HALL_MUX_A2 19

#define HALL_ADC_CHANNEL 1
#define HALL_KEY_NUM 7
#define HALL_KEY_MUX_MAP { 4, 5, 3, 6, 2, 0, 1 }

#elif defined BOARD_IIDX_TIMSUO_V1
#define BOARD_HAS_HALL_KEYS 0
#define BOARD_HAS_SECONDARY_TT_I2C 0
#define BOARD_HAS_ALT_TT_RGB 0
#define BOARD_HAS_LCD 1

/* KEY1..KEY7, then E1..E4. */
#define BUTTON_DEF { 8, 7, 6, 5, 4, 3, 2, 12, 11, 10, 9 }

#define BUTTON_RGB_PIN 13
#define BUTTON_RGB_ORDER GRB // or RGB

#define BUTTON_RGB_NUM 11
#define BUTTON_RGB_MAP { 6, 0, 5, 1, 4, 2, 3, 7, 8, 9, 10 }

#define TT_RGB_PIN 28
#define TT_RGB_ORDER GRB // or RGB

#define TT_SENSOR_SCL 17
#define TT_SENSOR_SDA 16
#define TT_SENSOR_I2C i2c0

#define TT_SENSOR_I2C_FREQ 377 * 1000

/* Reserved for a possible future quadrature encoder backend. */
#define TT_ENCODER_A_PIN 14
#define TT_ENCODER_B_PIN 15

#define LCD_SPI spi0
#define LCD_SCK_PIN 18
#define LCD_MOSI_PIN 19
#define LCD_CS_PIN 20
#define LCD_RST_PIN 21
#define LCD_DC_PIN 22

#else
#error "Unsupported IIDX controller board"
#endif
