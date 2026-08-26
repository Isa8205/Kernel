#include "../idt.h"
#include "../util.h"
#include "../vga.h"
#include <stdbool.h>
#include <stdint.h>

#define SCAN_LSHIFT 0x2A
#define SCAN_RSHIFT 0x36
#define SCAN_CTRL 0x1D // Left Ctrl. Right Ctrl is E0, 1D
#define SCAN_ALT 0x38  // Left Alt. Right Alt is E0, 38
#define SCAN_CAPSLOCK 0x3A
#define SCAN_NUMLOCK 0x45

typedef struct {
  bool shift_pressed;
  bool ctrl_pressed;
  bool alt_pressed;
  bool caps_lock;
  bool num_lock;
  bool e0_extended; // If the previous byte was 0xE0
} KeyboardState;

const char base_layout[128] = {
  0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', /* 0x00 - 0x0E (Esc, Numbers, Backspace) */
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     /* 0x0F - 0x1C (Tab, QWERTY, Enter) */
  0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  0,      /* 0x1D - 0x2A (Ctrl, ASDF, LShift) */
  '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0, '*',   0,      /* 0x2B - 0x38 (ZXC, RShift, PrintScrn, LAlt) */
  ' ',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,      /* 0x39 - 0x46 (Space, Caps, F1-F10) */
  // ... remaining indices default to 0
};

// The Shifted Layout (Shift key held down)
const char shift_layout[128] = {
  0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', 
  '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',     
  0,  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~',  0,      
  '|',  'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',   0, '*',   0,      
  ' ',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,      
};


KeyboardState kb_state = {false, false, false, false, false, false};

void keyboard_callback(void);

void keyboard_init(void) {
  irq_register_handler(1, (void *)keyboard_callback);
  kprint("[*] Keyboard Initialization done");
}

void keyboard_callback(void) {
  uint8_t scancode = read_port(0x60);


  if (scancode == 0xE0) {
    kb_state.e0_extended = 0;
    return;
  }

  // print the scancode
  
  bool is_release = (scancode & (1 << 7)) != 0;
  uint8_t actual_key = scancode & ~(1 << 7); // Strip the highest bit
  // print_hex((char *)&actual_key, "Actual Key: ");

  if (is_release) {
    if (actual_key == SCAN_LSHIFT || actual_key == SCAN_RSHIFT) {
      kb_state.shift_pressed = false;
    } else if (actual_key == SCAN_CTRL) {
      kb_state.ctrl_pressed = false;
    } else if (actual_key == SCAN_ALT) {
      kb_state.alt_pressed = false;
    }

    kb_state.e0_extended = false;
    return;
  }

  if (actual_key == SCAN_LSHIFT || actual_key == SCAN_RSHIFT) {
    kb_state.shift_pressed = true;
    return;
  } else if (actual_key == SCAN_CTRL) {
    kb_state.ctrl_pressed = true;
    return;
  } else if (actual_key == SCAN_ALT) {
    kb_state.alt_pressed = true;
    return;
  }

  if (actual_key == SCAN_CAPSLOCK) {
    kb_state.caps_lock = kb_state.caps_lock ? false : true;
    return;
  }
  if (actual_key == SCAN_NUMLOCK) {
    kb_state.num_lock = kb_state.num_lock ? false : true;
    return;
  }

  char final_char = 0;

  if (actual_key < 128) {
    if (kb_state.shift_pressed) {
      final_char = shift_layout[actual_key];
    } else {
      final_char = base_layout[actual_key];
    }
  }

  // Handle caps_lock for only letters
  if (final_char >= 0x61 && final_char <= 0x7A) {
    final_char = final_char - 32;
  }
  if (final_char >= 0x41 && final_char <= 0x5A) {
    final_char = final_char + 32;
  }

  if (final_char != 0 && !is_release) {
    char output[2] = { final_char, '\0'};
    kprint(output);
  }

  kb_state.e0_extended = false;
}
