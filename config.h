#ifndef CONFIG_H_
#define CONFIG_H_

/* Device identifiers */
#define PROGRAM_NAME            "Picophonica"
#define PROGRAM_VERSION         "1.0.0"
#define PROGRAM_DESCRIPTION     "Synth engine for a toy keyboard, with instrument presets and Midi out"
#define PROGRAM_URL             "https://turiscandurra.com/circuits"
#define USB_STR_MANUFACTURER    "TuriScandurra"
#define USB_STR_PRODUCT         "Picophonica"

/* Definitions */
#define NUM_KEYS                37
#define CAPO_DEFAULT            48

/* GPIO definitions */
#define AUDIO_PIN               28 // Note: also hardcoded in pico_synth_ex.c
#define AUDIO_PIN_DESCRIPTION   "Audio out"

// The main keybed
#define KEYPAD_0_COLS           {2, 3, 4, 5, 6, 7, 8, 9}
#define KEYPAD_0_ROWS           {14, 13, 12, 11, 10}
// Secondary buttons
#define KEYPAD_1_COLS           {22, 21, 20, 19, 18, 17, 16}
#define KEYPAD_1_ROWS           {27, 26}
// Please see pico_synth_ex.c for synth control message to key id mapping

#define POWER_ON_LED_PIN        PICO_DEFAULT_LED_PIN
#define POWER_ON_LED_PIN_DESCRIPTION        "Power-on LED"

/* Synth control message to key ID mapping */
#define NUM_KEYS_1              14

// On my keypad, key IDs on the secondary keypad are
// arranged as follows on page_0:
// 0   1   2   3   4        5   6
// 7   8   9   10  11       12  13
// The rightmost keys alter the value of page_shift,
// so we can map these IDs to the same layout:
// page_1:
// 14  15  16  17  18       19  x
// 21  22  23  24  25       26  x
// page_2:
// 28  29  30  31  32       33  x
// 35  36  37  38  39       40  x

// Keys are grouped into pairs
#define KEY_A_UP                0
#define KEY_A_DOWN              7
#define KEY_B_UP                1
#define KEY_B_DOWN              8
#define KEY_C_UP                2
#define KEY_C_DOWN              9
#define KEY_D_UP                3
#define KEY_D_DOWN              10
#define KEY_E_UP                4
#define KEY_E_DOWN              11
#define KEY_F_UP                5
#define KEY_F_DOWN              12
#define KEY_G_UP                6
#define KEY_G_DOWN              13
// page_0:
#define PRESET_1                KEY_A_UP
#define PRESET_6                KEY_A_DOWN
#define PRESET_2                KEY_B_UP
#define PRESET_7                KEY_B_DOWN
#define PRESET_3                KEY_C_UP
#define PRESET_8                KEY_C_DOWN
#define PRESET_4                KEY_D_UP
#define PRESET_9                KEY_D_DOWN
#define PRESET_5                KEY_E_UP
#define PRESET_0                KEY_E_DOWN
#define OCTAVE_SHIFT_INC        KEY_F_UP
#define OCTAVE_SHIFT_DEC        KEY_F_DOWN
// page_1:
#define EG_SUSTAIN_LEVEL_INC    KEY_A_UP   + NUM_KEYS_1
#define EG_SUSTAIN_LEVEL_DEC    KEY_A_DOWN + NUM_KEYS_1
#define OSC_2_COARSE_PITCH_INC  KEY_B_UP   + NUM_KEYS_1
#define OSC_2_COARSE_PITCH_DEC  KEY_B_DOWN + NUM_KEYS_1
#define OSC_2_FINE_PITCH_INC    KEY_C_UP   + NUM_KEYS_1
#define OSC_2_FINE_PITCH_DEC    KEY_C_DOWN + NUM_KEYS_1
#define OSC_1_2_MIX_INC         KEY_D_UP   + NUM_KEYS_1
#define OSC_1_2_MIX_DEC         KEY_D_DOWN + NUM_KEYS_1
#define EG_DECAY_TIME_INC       KEY_E_UP   + NUM_KEYS_1
#define EG_DECAY_TIME_DEC       KEY_E_DOWN + NUM_KEYS_1
// page_2:
#define FILTER_CUTOFF_INC       KEY_A_UP    + NUM_KEYS_1 * 2
#define FILTER_CUTOFF_DEC       KEY_A_DOWN  + NUM_KEYS_1 * 2
#define FILTER_RESONANCE_INC    KEY_B_UP    + NUM_KEYS_1 * 2
#define FILTER_RESONANCE_DEC    KEY_B_DOWN  + NUM_KEYS_1 * 2
#define FILTER_MOD_AMOUNT_INC   KEY_C_UP    + NUM_KEYS_1 * 2
#define FILTER_MOD_AMOUNT_DEC   KEY_C_DOWN  + NUM_KEYS_1 * 2
#define LFO_DEPTH_INC           KEY_D_UP    + NUM_KEYS_1 * 2
#define LFO_DEPTH_DEC           KEY_D_DOWN  + NUM_KEYS_1 * 2
#define LFO_RATE_INC            KEY_E_UP    + NUM_KEYS_1 * 2
#define LFO_RATE_DEC            KEY_E_DOWN  + NUM_KEYS_1 * 2

#define ALL_NOTES_OFF           20

#define NUM_PAGES               3

/* Debugging */
#define DEBUG                   1

#endif /* CONFIG_H_ */