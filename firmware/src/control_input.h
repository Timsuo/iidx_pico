/*
 * Setup and controller-local input processing.
 */

#ifndef CONTROL_INPUT_H
#define CONTROL_INPUT_H

#include <stdint.h>

#define CONTROL_E1_BIT 7
#define CONTROL_E2_BIT 8
#define CONTROL_E3_BIT 9
#define CONTROL_E4_BIT 10
#define CONTROL_AUX_NO_BIT 11
#define CONTROL_AUX_YES_BIT 12

#define CONTROL_E1 (1u << CONTROL_E1_BIT)
#define CONTROL_E2 (1u << CONTROL_E2_BIT)
#define CONTROL_E3 (1u << CONTROL_E3_BIT)
#define CONTROL_E4 (1u << CONTROL_E4_BIT)
#define CONTROL_AUX_NO (1u << CONTROL_AUX_NO_BIT)
#define CONTROL_AUX_YES (1u << CONTROL_AUX_YES_BIT)

#define CONTROL_E_MASK (CONTROL_E1 | CONTROL_E2 | CONTROL_E3 | CONTROL_E4)

uint16_t control_input_update(uint16_t raw_buttons);

#endif
