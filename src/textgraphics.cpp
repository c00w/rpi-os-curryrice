#include "textgraphics.h" 
#include "x86/x86.h"

namespace text_graphics
{
  uint8_t text_attribute = GREEN_FOREGROUND;
  uint cursor_position = 0;

  static inline void update_cursor_pos();
  static inline int8_t* start_vid_mem();
  static inline int8_t* end_vid_mem();

  void init()
  {
    clear();
    hide_cursor();
  }

  uint rows()
  { return 25; } 

  uint cols()
  { return 80; }

  uint8_t text_attr()
  { return text_attribute; }

  void set_text_attr(uint8_t a)
  { text_attribute = a; }

  void clear()
  {
    for (uint pos = 0; pos < rows()*cols(); ++pos)
      put_char(' ', pos);
  }

  void put_char(int8_t c, uint position)
  {
    *(start_vid_mem() + position*2) = static_cast<int8_t>(c);
    *(start_vid_mem() + position*2 + 1) = text_attr();
  }

  void put_char(int8_t c, const point2d& position)
  { put_char(c, position.y*cols() + position.x); }

  int8_t char_at(uint position)
  { return *(start_vid_mem() + position*2); }

  int8_t char_at(const point2d& position)
  { return char(position.y*cols() + position.x); }

  void set_cursor_pos(uint position)
  {
    cursor_position = position;
    update_cursor_pos();
  }

  void set_cursor_pos(const point2d& position)
  { set_cursor_pos(position.x + position.y*cols()); }

  void hide_cursor()
  { set_cursor_pos(~0); }

  uint cursor_pos()
  { return cursor_position; }

  static inline void update_cursor_pos()
  {
    // Write cursor location low byte index to CRT index register
    x86::OUTB(0x03D4, 0x0F);

    // Write new low byte of cursor position to CRT data register
    x86::OUTB(0x3D5, static_cast<uint8_t>((cursor_position) & 0xFF));

    // Write cursor location high byte index to CRT index register
    x86::OUTB(0x03D4, 0x0E);

    // Write new high byte of cursor position to CRT data register
    x86::OUTB(0x3D5, static_cast<uint8_t>((cursor_position >> 8) & 0xFF));
  }

  static inline int8_t* start_vid_mem()
  { return (int8_t*)0xB8000; }

  static inline int8_t* end_vid_mem()
  { return (int8_t*)0xBFFFF; }
}
