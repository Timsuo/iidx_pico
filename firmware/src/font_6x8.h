/*
 * Compact MCU-side 6x8 ASCII font.
 */

#ifndef FONT_6X8_H
#define FONT_6X8_H

#include <stdint.h>

#define FONT_6X8_WIDTH 6
#define FONT_6X8_HEIGHT 8

const uint8_t *font_6x8_glyph(char character);

#endif
