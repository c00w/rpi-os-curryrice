#pragma once
#include "common.h"

enum KEY
{
  KEY_A,
  KEY_B,
  KEY_C,
  KEY_D,
  KEY_E,
  KEY_F,
  KEY_G,
  KEY_H,
  KEY_I,
  KEY_J,
  KEY_K,
  KEY_L,
  KEY_M,
  KEY_N,
  KEY_O,
  KEY_P,
  KEY_Q,
  KEY_R,
  KEY_S,
  KEY_T,
  KEY_U,
  KEY_V,
  KEY_W,
  KEY_X,
  KEY_Y,
  KEY_Z,

  KEY_0,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
  KEY_9,

  KEY_MINUS,
  KEY_EQUAL,
  KEY_LEFT_BRACKET,
  KEY_RIGHT_BRACKET,
  KEY_SEMICOLON,
  KEY_QUOTE,
  KEY_COMMA,
  KEY_DOT,
  KEY_SLASH,
  KEY_BACKSLASH,
  KEY_GRAVE,

  KEY_SPACE,
  KEY_TAB,
  KEY_RETURN,
  KEY_BACKSPACE,

  // Non-printable
  KEY_ESCAPE,

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
  KEY_F11,
  KEY_F12,
  KEY_F13,
  KEY_F14,
  KEY_F15,

  KEY_KP_NUM_LOCK,
  KEY_CAPS_LOCK,
  KEY_LSHIFT,
  KEY_LCTRL,
  KEY_LALT,
  KEY_LWIN,
  KEY_RSHIFT,
  KEY_RCTRL,
  KEY_RALT,
  KEY_RWIN,

  KEY_INSERT,
  KEY_DELETE,
  KEY_HOME,
  KEY_END,
  KEY_PAGE_UP,
  KEY_PAGE_DOWN,
  KEY_SCROLL_LOCK,
  KEY_PAUSE,

  KEY_KP_ASTERISK,
  KEY_KP_DECIMAL,
  KEY_KP_8,
  KEY_KP_2,
  KEY_KP_3,
  KEY_KP_0,

  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT
};

extern const int8_t* key_names[];

inline bool is_digit(int8_t ch)
{ return ch >= '0' && ch <= '9'; }

inline bool is_upper(int8_t ch)
{ return ch >= 'A' && ch <= 'Z'; }

inline bool is_whitespace(int8_t ch)
{ return ch == ' '  || 
         ch == '\f' || 
         ch == '\n' ||
         ch == '\r' ||
         ch == '\t' ||
         ch == '\b' ||
         ch == '\v'; }

inline bool is_key_printable(KEY key)
{ return key < KEY_ESCAPE; }

const static uint keyToCharTable[] =
{
  'a', // KEY_A
  'b', // KEY_B
  'c', // KEY_C
  'd', // KEY_D
  'e', // KEY_E
  'f', // KEY_F
  'g', // KEY_G
  'h', // KEY_H
  'i', // KEY_I
  'j', // KEY_J
  'k', // KEY_K
  'l', // KEY_L
  'm', // KEY_M
  'n', // KEY_N
  'o', // KEY_O
  'p', // KEY_P
  'q', // KEY_Q
  'r', // KEY_R
  's', // KEY_S
  't', // KEY_T
  'u', // KEY_U
  'v', // KEY_V
  'w', // KEY_W
  'x', // KEY_X
  'y', // KEY_Y
  'z', // KEY_Z

  '0', // KEY_0
  '1', // KEY_1
  '2', // KEY_2
  '3', // KEY_3
  '4', // KEY_4
  '5', // KEY_5
  '6', // KEY_6
  '7', // KEY_7
  '8', // KEY_8
  '9', // KEY_9

  '-', // KEY_MINUS
  '=', // KEY_EQUAL
  '[', // KEY_LEFT_BRACKET
  ']', // KEY_RIGHT_BRACKET
  ';', // KEY_SEMICOLON
  '\'', // KEY_QUOTE
  ',', // KEY_COMMA
  '.', // KEY_DOT
  '/', // KEY_SLASH
  '\\', // KEY_BACKSLASH
  '`', // KEY_GRAVE

  ' ', // KEY_SPACE
  '\t', // KEY_TAB
  '\n', // KEY_RETURN
  '\b' // KEY_BACKSPACE
};

const static uint shiftedKeyToCharTable[] =
{
  'A', // KEY_A
  'B', // KEY_B
  'C', // KEY_C
  'D', // KEY_D
  'E', // KEY_E
  'F', // KEY_F
  'G', // KEY_G
  'H', // KEY_H
  'I', // KEY_I
  'J', // KEY_J
  'K', // KEY_K
  'L', // KEY_L
  'M', // KEY_M
  'N', // KEY_N
  'O', // KEY_O
  'P', // KEY_P
  'Q', // KEY_Q
  'R', // KEY_R
  'S', // KEY_S
  'T', // KEY_T
  'U', // KEY_U
  'V', // KEY_V
  'W', // KEY_W
  'X', // KEY_X
  'Y', // KEY_Y
  'Z', // KEY_Z

  ')', // KEY_0
  '!', // KEY_1
  '@', // KEY_2
  '#', // KEY_3
  '$', // KEY_4
  '%', // KEY_5
  '^', // KEY_6
  '&', // KEY_7
  '*', // KEY_8
  '(', // KEY_9

  '_', // KEY_MINUS
  '+', // KEY_EQUAL
  '{', // KEY_LEFT_BRACKET
  '}', // KEY_RIGHT_BRACKET
  ':', // KEY_SEMICOLON
  '\"', // KEY_QUOTE
  '<', // KEY_COMMA
  '>', // KEY_DOT
  '?', // KEY_SLASH
  '|', // KEY_BACKSLASH
  '~', // KEY_GRAVE

  ' ', // KEY_SPACE
  '\t', // KEY_TAB
  '\n', // KEY_RETURN
  '\b' // KEY_BACKSPACE
};
