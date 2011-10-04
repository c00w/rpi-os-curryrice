#include "x86/x86.h"
#include "x86/int.h"
#include "textgraphics.h"

namespace keyboard
{
  enum CONTROLLER_STATUS
  {
    CONTROLLER_STATUS_OUT_BUF  = 1 << 0,
    CONTROLLER_STATUS_IN_BUF   = 1 << 1,
    CONTROLLER_STATUS_SYSTEM   = 1 << 2,
    CONTROLLER_STATUS_CMD_DATA = 1 << 3,
    CONTROLLER_STATUS_LOCKED   = 1 << 4,
    CONTROLLER_STATUS_AUX_BUF  = 1 << 5,
    CONTROLLER_STATUS_TIMEOUT  = 1 << 6,
    CONTROLLER_STATUS_PARITY   = 1 << 7
  };

  enum ENCODER_IO_PORT
  {
    ENCODER_IO_PORT_INPUT_BUFFER = 0x60,
    ENCODER_IO_PORT_CMD_REG      = 0x60
  };

  enum CONTROLLER_IO_PORT
  {
    CONTROLLER_IO_PORT_STATUS_REG = 0x64,
    CONTROLLER_IO_PORT_CMD_REG    = 0x64
  };

  enum ENCODER_COMMAND
  {
    ENCODER_COMMAND_SET_LEDS                                          = 0xED,
    ENCODER_COMMAND_ECHO                                              = 0xEE,
    ENCODER_COMMAND_SET_ALTERNATE_SCAN_CODE                           = 0xF0,
    ENCODER_COMMAND_SEND_2KEYBOARD_ID_CODE                            = 0xF2,
    ENCODER_COMMAND_SET_AUTO_REPEAT_RATE_AND_DELAY                    = 0xF3,
    ENCODER_COMMAND_ENABLE_KEYBOARD                                   = 0xF4,
    ENCODER_COMMAND_RESET_TO_POWER_ON_WAIT_FOR_ENABLE                 = 0xF5,
    ENCODER_COMMAND_RESET_AND_SCAN                                    = 0xF6,
    ENCODER_COMMAND_SET_ALL_AUTOREPEAT_PS2                            = 0xF7,
    ENCODER_COMMAND_SET_SEND_MAKE_AND_BREAK_CODES_PS2                 = 0xF8,
    ENCODER_COMMAND_SET_SEND_MAKE_CODES                               = 0xF9,
    ENCODER_COMMAND_SET_ALL_AUTOREAPEAT_AND_SEND_MAKE_AND_BREAK_CODES = 0xFA,
    ENCODER_COMMAND_SET_KEY_AUTOREPEAT                                = 0xFB,
    ENCODER_COMMAND_SET_KEY_SEND_MAKE_AND_BREAK_CODES                 = 0xFC,
    ENCODER_COMMAND_SET_KEY_SEND_BREAK_CODE                           = 0xFD,
    ENCODER_COMMAND_RESEND                                            = 0xFE,
    ENCODER_COMMAND_RESET_AND_SELF_TEST                               = 0xFF
  };

  enum CONTROLLER_COMMAND
  {
    CONTROLLER_COMMAND_READ             = 0x20,
    CONTROLLER_COMMAND_WRITE            = 0x60,
    CONTROLLER_COMMAND_SELF_TEST        = 0xAA,
    CONTROLLER_COMMAND_INTERFACE_TEST   = 0xAB,
    CONTROLLER_COMMAND_DISABLE          = 0xAD,
    CONTROLLER_COMMAND_ENABLE           = 0xAE,
    CONTROLLER_COMMAND_READ_IN_PORT     = 0xC0,
    CONTROLLER_COMMAND_READ_OUT_PORT    = 0xD0,
    CONTROLLER_COMMAND_WRITE_OUT_PORT   = 0xD1,
    CONTROLLER_COMMAND_READ_TEST_INPUTS = 0xE0,
    CONTROLLER_COMMAND_SYSTEM_RESET     = 0xFE,
    CONTROLLER_COMMAND_MOUSE_DISABLE    = 0xA7,
    CONTROLLER_COMMAND_MOUSE_ENABLE     = 0xA8,
    CONTROLLER_COMMAND_MOUSE_PORT_TEST  = 0xA9,
    CONTROLLER_COMMAND_MOUSE_WRITE      = 0xD4
  };

  enum SCAN_ERROR_CODE
  {
    SCAN_ERROR_CODE_BUF_OVERRUN = 0,
    SCAN_ERROR_CODE_ID_RET      = 0x83AB,
    SCAN_ERROR_CODE_BAT         = 0xAA,
    SCAN_ERROR_CODE_ECHO_RET    = 0xEE,
    SCAN_ERROR_CODE_ACK         = 0xFA,
    SCAN_ERROR_CODE_BAT_FAILED  = 0xFC,
    SCAN_ERROR_CODE_DIAG_FAILED = 0xFD,
    SCAN_ERROR_CODE_RESEND_CMD  = 0xFE,
    SCAN_ERROR_CODE_KEY         = 0xFF
  };

  // Original XT scan code set
  static int MakeCodesToKeys[] =
  {
    -1,
    KEY_ESCAPE,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_0,
    KEY_MINUS,
    KEY_EQUAL,
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_Q,
    KEY_W,
    KEY_E,
    KEY_R,
    KEY_T,
    KEY_Y,
    KEY_U,
    KEY_I,
    KEY_O,
    KEY_P,
    KEY_LEFT_BRACKET,
    KEY_RIGHT_BRACKET,
    KEY_RETURN,
    KEY_LCTRL,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_SEMICOLON,
    KEY_QUOTE,
    KEY_GRAVE,
    KEY_LSHIFT,
    KEY_BACKSLASH,
    KEY_Z,
    KEY_X,
    KEY_C,
    KEY_V,
    KEY_B,
    KEY_N,
    KEY_M,
    KEY_COMMA,
    KEY_DOT,
    KEY_SLASH,
    KEY_RSHIFT,
    KEY_KP_ASTERISK,
    KEY_RALT,
    KEY_SPACE,
    KEY_CAPS_LOCK,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_KP_NUM_LOCK,
    KEY_SCROLL_LOCK,
    KEY_HOME,
    KEY_UP,
    KEY_PAGE_UP,
    KEY_KP_2,
    KEY_LEFT,
    KEY_KP_0,
    KEY_RIGHT,
    -1,
    -1,
    KEY_DOWN,
    KEY_PAGE_DOWN,
    KEY_F11,
    KEY_F12
  };

  uint8_t ledMask = 0;
  bool is_caps_lock = false, is_num_lock = false, is_scroll_lock = false, is_shift = false;
  int k = -1;
  int c = -1;

  static inline uint8_t read_controller_status()
  { return x86::INB(CONTROLLER_IO_PORT_STATUS_REG); }

  static inline uint8_t read_encoder_buffer()
  { return x86::INB(ENCODER_IO_PORT_INPUT_BUFFER); }

  static inline void WaitToSendCommand()
  {
    while (1)
    {
      if ((read_controller_status () & CONTROLLER_STATUS_IN_BUF) == 0)
	break;
    }
  }

  static inline void SendControllerCommand(uint8_t command)
  {
    WaitToSendCommand();
    x86::OUTB(CONTROLLER_IO_PORT_CMD_REG, command);
  }

  static inline void SendEncoderCommand(uint8_t command)
  {
    WaitToSendCommand();
    x86::OUTB(ENCODER_IO_PORT_CMD_REG, command);
  }

  static inline void UpdateLEDs()
  {
    SendEncoderCommand(ENCODER_COMMAND_SET_LEDS);
    SendEncoderCommand(static_cast<uint8_t>(ledMask));
  }

  void enable()
  { SendControllerCommand(CONTROLLER_COMMAND_ENABLE); }

  void init()
  {
    x86::idt::install_interrupt((x86::idt::interrupt_proc)irq_1, 33, x86::idt::DESCRIPTOR_FLAG_BIT32 | x86::idt::DESCRIPTOR_FLAG_PRESENT, x86::gdt::DEFAULT_CODE_SELECTOR);
    enable();
  }

  void irq()
  {
    bool is_break = false;
    KEY key;

    // Read scan code only if the kkybrd controller output buffer is full
    if (!(read_controller_status() & CONTROLLER_STATUS_OUT_BUF))
      return;

    // Read the scan code
    uint code = read_encoder_buffer();

    // Is this not an extended code?
    if (code != 0xE0 && code != 0xE1)
    {
      // Either the second byte of an extended scan code or a single byte scan code

      // Test if this is a break code (Original XT Scan Code Set specific)
      if (code & 0x80)
      {
	// Convert the break code into its make code equivelant
	code -= 0x80;
	is_break = true;
      }

      // Get key
      key = static_cast<KEY>(MakeCodesToKeys[code]);

      if (is_break)
      {
        // Key release
      }
      else
      {
        // Key press
	k = key;

	if (is_key_printable(key))
	{
	  int8_t character;

	  if (is_shift)
	    character = shiftedKeyToCharTable[key];
	  else
	    character = keyToCharTable[key];

	  c = character;
	}
      }

      switch (key)
      {
	case KEY_LSHIFT:
	case KEY_RSHIFT:
	  is_shift = !is_break;
	  break;

	case KEY_CAPS_LOCK:
	  is_caps_lock = !is_caps_lock;
	  break;

	case KEY_KP_NUM_LOCK:
	  is_num_lock = !is_break;
	  break;

	case KEY_SCROLL_LOCK:
	  is_scroll_lock = !is_break;
	  break;

	default:
	  break;
      }
    }
  }

  void Disable()
  { SendControllerCommand(CONTROLLER_COMMAND_DISABLE); }

  KEY get_key()
  {
    while (k == -1);
    KEY r = static_cast<KEY>(k);
    k = -1;
    return r;
  }

  uint get_char()
  {
    while (c == -1);
    uint r = c;
    c = -1;
    return r;
  }
}
