/*
 * Setup and controller-local input processing.
 */

#include "control_input.h"

#include <stdbool.h>

#include "pico/time.h"

#include "board_defs.h"

#if !BOARD_HAS_PHYSICAL_AUX_KEYS

#define CHORD_WINDOW_US 40000

typedef enum {
    E_STATE_IDLE,
    E_STATE_CANDIDATE,
    E_STATE_RAW,
    E_STATE_CHORD,
} e_state_t;

static e_state_t e_state;
static uint16_t e_candidate;
static uint16_t e_chord_output;
static uint64_t e_deadline;

static uint16_t resolve_e_candidate(uint16_t e_buttons)
{
    switch (e_buttons & CONTROL_E_MASK) {
        case CONTROL_E1 | CONTROL_E2:
            return CONTROL_AUX_YES;
        case CONTROL_E3 | CONTROL_E4:
            return CONTROL_AUX_NO;
        case CONTROL_E_MASK:
            return CONTROL_AUX_YES | CONTROL_AUX_NO;
        default:
            return e_buttons & CONTROL_E_MASK;
    }
}

#endif

uint16_t control_input_update(uint16_t raw_buttons)
{
#if BOARD_HAS_PHYSICAL_AUX_KEYS
    return raw_buttons;
#else
    uint16_t non_e_buttons = raw_buttons & ~CONTROL_E_MASK;
    uint16_t e_buttons = raw_buttons & CONTROL_E_MASK;
    uint64_t now = time_us_64();

    switch (e_state) {
        case E_STATE_IDLE:
            if (e_buttons != 0) {
                e_candidate = e_buttons;
                e_deadline = now + CHORD_WINDOW_US;
                e_state = E_STATE_CANDIDATE;
            }
            return non_e_buttons;

        case E_STATE_CANDIDATE:
            e_candidate |= e_buttons;
            if (e_buttons == 0) {
                uint16_t pulse = resolve_e_candidate(e_candidate);
                e_state = E_STATE_IDLE;
                return non_e_buttons | pulse;
            }
            if (now < e_deadline) {
                return non_e_buttons;
            }

            e_chord_output = resolve_e_candidate(e_candidate);
            if (e_chord_output & (CONTROL_AUX_YES | CONTROL_AUX_NO)) {
                e_state = E_STATE_CHORD;
                return non_e_buttons | e_chord_output;
            }

            e_state = E_STATE_RAW;
            return non_e_buttons | e_buttons;

        case E_STATE_RAW:
            if (e_buttons == 0) {
                e_state = E_STATE_IDLE;
            }
            return non_e_buttons | e_buttons;

        case E_STATE_CHORD:
            if (e_buttons == 0) {
                e_state = E_STATE_IDLE;
                return non_e_buttons;
            }
            return non_e_buttons | e_chord_output;
    }

    e_state = E_STATE_IDLE;
    return non_e_buttons;
#endif
}
