/*
 * IIDX controller status screen renderer.
 */

#include "status_screen.h"

#include <stdio.h>

#include "config.h"
#include "display.h"
#include "turntable.h"

#define E1_BIT 7

static void format_button_state(char *output, uint16_t buttons,
                                uint8_t first_bit, uint8_t count)
{
    for (uint8_t i = 0; i < count; i++) {
        output[i] = (buttons & (1u << (first_bit + i))) ? '#' : '.';
    }
    output[count] = '\0';
}

void status_screen_render(uint16_t buttons, uint8_t turntable_position)
{
    char line[22];
    char key_state[8];
    char effect_state[5];

    format_button_state(key_state, buttons, 0, 7);
    format_button_state(effect_state, buttons, E1_BIT, 4);

    display_clear();
    display_draw_text(0, 0, "IIDX TIMSUO V1");
    snprintf(line, sizeof(line), "MODE %s",
             iidx_cfg->hid.konami ? "KONAMI" : "BEATORAJA");
    display_draw_text(0, 8, line);
    snprintf(line, sizeof(line), "PROFILE %u", (iidx_cfg->profile % 4) + 1);
    display_draw_text(0, 16, line);
    snprintf(line, sizeof(line), "TT %s", turntable_sensor_name());
    display_draw_text(0, 24, line);
    snprintf(line, sizeof(line), "PPR %u", turntable_ppr());
    display_draw_text(0, 32, line);
    snprintf(line, sizeof(line), "K:%s E:%s", key_state, effect_state);
    display_draw_text(0, 40, line);
    snprintf(line, sizeof(line), "TT POS %u", turntable_position);
    display_draw_text(0, 48, line);
}
