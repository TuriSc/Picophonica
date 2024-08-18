/* Picophonica
 * A Raspberry Pi Pico synth engine for a toy keyboard, with instrument presets and Midi out.
 * By Turi Scandurra – https://turiscandurra.com/circuits
 * v1.0.0 - 2023.11.02
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/clocks.h" 

#include "config.h"

#include "bsp/board_api.h"      // For Midi
#include "tusb.h"               // For Midi
#include "inc/pico_synth_ex.c"  // Synth by ISGK Instruments (Ryo Ishigaki), licensed under CC0
#include "keypad.h"             // https://github.com/TuriSc/RP2040-Keypad-Matrix

KeypadMatrix keypad_0; // The main keybed
const uint8_t cols_0[] = KEYPAD_0_COLS;
const uint8_t rows_0[] = KEYPAD_0_ROWS;

KeypadMatrix keypad_1; // Secondary buttons
const uint8_t cols_1[] = KEYPAD_1_COLS;
const uint8_t rows_1[] = KEYPAD_1_ROWS;

uint8_t capo = CAPO_DEFAULT; // The first note on the keyboard
uint8_t page_shift;
bool mute;

static inline uint32_t tudi_midi_write24 (uint8_t jack_id, uint8_t b1, uint8_t b2, uint8_t b3){
    uint8_t msg[3] = { b1, b2, b3 };
    return tud_midi_stream_write(jack_id, msg, 3);
}

void keypad_0_pressed(uint8_t key){
    if(!mute) { note_on(capo + key); }
    int8_t octave_shift = get_octave_shift() * 12;
    tudi_midi_write24(0, 0x90, capo + key + octave_shift, 100); // 100 is an arbitrary velocity value
}

void keypad_0_released(uint8_t key){
    note_off(capo + key);
    int8_t octave_shift = get_octave_shift() * 12;
    tudi_midi_write24(0, 0x80, capo + key + octave_shift, 0);
}

void keypad_1_pressed(uint8_t key){
    switch(key){
        case KEY_F_UP:
            synth_control(OCTAVE_SHIFT_INC);
        break;
        case KEY_F_DOWN:
            synth_control(OCTAVE_SHIFT_DEC);
        break;
        case KEY_G_UP:
            if(++page_shift >= NUM_PAGES){ page_shift = 0; }
        break;
        case KEY_G_DOWN:
            if(--page_shift < 0){ page_shift = NUM_PAGES; }
        break;
        default:
            synth_control(key + page_shift * NUM_KEYS_1);
        break;
    }
    // print_status(); // DEBUG
}

void keypad_1_long_pressed(uint8_t key){
    // TODO: hold a key while on page 0 to save to flash
    // current settings as a preset
    switch(key){
        case KEY_A_UP:
        case KEY_A_DOWN:
        case KEY_B_UP:
        case KEY_B_DOWN:
        case KEY_C_UP:
        case KEY_C_DOWN:
        case KEY_D_UP:
        case KEY_D_DOWN:
        case KEY_E_UP:
        case KEY_E_DOWN:
            // Long-pressing a key equals pressing it six times
            for(uint8_t i=0; i<6; i++) {
                synth_control(key + page_shift * NUM_KEYS_1);
            }
        break;
        case KEY_G_UP:
            mute = !mute; // Muting the synth. Useful when using Midi
            synth_control(ALL_NOTES_OFF);
        break;
    }
}

void bi_decl_all(){
#if PICO_ON_DEVICE
    bi_decl(bi_program_name(PROGRAM_NAME));
    bi_decl(bi_program_description(PROGRAM_DESCRIPTION));
    bi_decl(bi_program_version_string(PROGRAM_VERSION));
    bi_decl(bi_program_url(PROGRAM_URL));
    bi_decl(bi_1pin_with_name(PWMA_L_GPIO, PWMA_L_GPIO_DESCRIPTION));
    bi_decl(bi_1pin_with_name(POWER_ON_LED_PIN, POWER_ON_LED_PIN_DESCRIPTION));
    bi_decl(bi_1pin_with_name(cols_0[0], "Keypad matrix 0 column pin 1"));
    bi_decl(bi_1pin_with_name(cols_0[1], "Keypad matrix 0 column pin 2"));
    bi_decl(bi_1pin_with_name(cols_0[2], "Keypad matrix 0 column pin 3"));
    bi_decl(bi_1pin_with_name(cols_0[3], "Keypad matrix 0 column pin 4"));
    bi_decl(bi_1pin_with_name(cols_0[4], "Keypad matrix 0 column pin 5"));
    bi_decl(bi_1pin_with_name(cols_0[5], "Keypad matrix 0 column pin 6"));
    bi_decl(bi_1pin_with_name(cols_0[6], "Keypad matrix 0 column pin 7"));
    bi_decl(bi_1pin_with_name(cols_0[7], "Keypad matrix 0 column pin 8"));
    bi_decl(bi_1pin_with_name(rows_0[0], "Keypad matrix 0 row pin 1"));
    bi_decl(bi_1pin_with_name(rows_0[1], "Keypad matrix 0 row pin 2"));
    bi_decl(bi_1pin_with_name(rows_0[2], "Keypad matrix 0 row pin 3"));
    bi_decl(bi_1pin_with_name(rows_0[3], "Keypad matrix 0 row pin 4"));
    bi_decl(bi_1pin_with_name(rows_0[4], "Keypad matrix 0 row pin 5"));
    bi_decl(bi_1pin_with_name(cols_1[0], "Keypad matrix 1 column pin 1"));
    bi_decl(bi_1pin_with_name(cols_1[1], "Keypad matrix 1 column pin 2"));
    bi_decl(bi_1pin_with_name(cols_1[2], "Keypad matrix 1 column pin 3"));
    bi_decl(bi_1pin_with_name(cols_1[3], "Keypad matrix 1 column pin 4"));
    bi_decl(bi_1pin_with_name(cols_1[4], "Keypad matrix 1 column pin 5"));
    bi_decl(bi_1pin_with_name(cols_1[5], "Keypad matrix 1 column pin 6"));
    bi_decl(bi_1pin_with_name(cols_1[6], "Keypad matrix 1 column pin 7"));
    bi_decl(bi_1pin_with_name(rows_1[0], "Keypad matrix 1 row pin 1"));
    bi_decl(bi_1pin_with_name(rows_1[1], "Keypad matrix 1 row pin 2"));
#endif
}

int main() {
    bi_decl_all();
    // Start the synth
    set_sys_clock_khz(FCLKSYS / 1000, true);
    stdio_init_all();
    startup_chord();
    PWMA_init();

    // Enable Midi
    board_init();
    tusb_init();

    // Configure the keypads
    keypad_init(&keypad_0, cols_0, rows_0, 8, 5);
    keypad_on_press(&keypad_0, keypad_0_pressed);
    keypad_on_release(&keypad_0, keypad_0_released);

    keypad_init(&keypad_1, cols_1, rows_1, 7, 2);
    keypad_on_press(&keypad_1, keypad_1_pressed);
    keypad_set_hold_threshold(&keypad_1, 500);
    keypad_on_long_press(&keypad_1, keypad_1_long_pressed);

    while (true) {
        keypad_read(&keypad_0);
        keypad_read(&keypad_1);
        tud_task(); // tinyusb device task
    }
}
