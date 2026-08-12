/*
 * 128x64 monochrome framebuffer and drawing primitives.
 */

#include "display.h"

#include <string.h>

#include "font_6x8.h"
#include "uc1701x.h"

static uint8_t framebuffer[DISPLAY_PAGES][DISPLAY_WIDTH];
static uint8_t dirty_pages;

_Static_assert(sizeof(framebuffer) == 1024, "Display framebuffer must be 1024 bytes");
_Static_assert(DISPLAY_WIDTH == UC1701X_WIDTH, "Display and controller widths differ");
_Static_assert(DISPLAY_PAGES == UC1701X_PAGE_COUNT, "Display and controller page counts differ");

void display_clear(void)
{
    memset(framebuffer, 0, sizeof(framebuffer));
    dirty_pages = 0xff;
}

void display_set_pixel(int x, int y, bool on)
{
    if ((x < 0) || (x >= DISPLAY_WIDTH) || (y < 0) || (y >= DISPLAY_HEIGHT)) {
        return;
    }

    uint8_t page = y / 8;
    uint8_t mask = 1u << (y % 8);
    uint8_t old_value = framebuffer[page][x];
    if (on) {
        framebuffer[page][x] |= mask;
    } else {
        framebuffer[page][x] &= ~mask;
    }
    if (framebuffer[page][x] != old_value) {
        dirty_pages |= 1u << page;
    }
}

void display_draw_hline(int x, int y, int width, bool on)
{
    for (int i = 0; i < width; i++) {
        display_set_pixel(x + i, y, on);
    }
}

void display_draw_vline(int x, int y, int height, bool on)
{
    for (int i = 0; i < height; i++) {
        display_set_pixel(x, y + i, on);
    }
}

void display_draw_rect(int x, int y, int width, int height, bool on)
{
    if ((width <= 0) || (height <= 0)) {
        return;
    }
    display_draw_hline(x, y, width, on);
    display_draw_hline(x, y + height - 1, width, on);
    display_draw_vline(x, y, height, on);
    display_draw_vline(x + width - 1, y, height, on);
}

void display_fill_rect(int x, int y, int width, int height, bool on)
{
    for (int row = 0; row < height; row++) {
        display_draw_hline(x, y + row, width, on);
    }
}

void display_draw_char(int x, int y, char character)
{
    const uint8_t *glyph = font_6x8_glyph(character);
    for (int column = 0; column < 5; column++) {
        for (int row = 0; row < 8; row++) {
            display_set_pixel(x + column, y + row, glyph[column] & (1u << row));
        }
    }
    display_draw_vline(x + 5, y, 8, false);
}

void display_draw_text(int x, int y, const char *text)
{
    if (text == NULL) {
        return;
    }
    while ((*text != '\0') && (x < DISPLAY_WIDTH)) {
        display_draw_char(x, y, *text++);
        x += FONT_6X8_WIDTH;
    }
}

void display_task(void)
{
    for (uint8_t page = 0; page < DISPLAY_PAGES; page++) {
        uint8_t mask = 1u << page;
        if (dirty_pages & mask) {
            uc1701x_write_page(page, framebuffer[page], DISPLAY_WIDTH);
            dirty_pages &= ~mask;
            return;
        }
    }
}

uint8_t display_dirty_pages(void)
{
    return dirty_pages;
}
