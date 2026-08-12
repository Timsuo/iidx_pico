/*
 * 128x64 monochrome framebuffer and drawing primitives.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stdint.h>

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_PAGES 8

void display_clear(void);
void display_set_pixel(int x, int y, bool on);
void display_draw_hline(int x, int y, int width, bool on);
void display_draw_vline(int x, int y, int height, bool on);
void display_draw_rect(int x, int y, int width, int height, bool on);
void display_fill_rect(int x, int y, int width, int height, bool on);
void display_draw_char(int x, int y, char character);
void display_draw_text(int x, int y, const char *text);

void display_task(void);
uint8_t display_dirty_pages(void);

#endif
