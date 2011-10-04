#pragma once
#include "common.h"
#include "vec2d.h"

namespace text_graphics
{
  enum TEXT_ATTRIBUTE_FLAGS
  {
    BLUE_FOREGROUND    = 1 << 0,
    GREEN_FOREGROUND   = 1 << 1,
    RED_FOREGROUND     = 1 << 2,
    INTENSE_FOREGROUND = 1 << 3,
    BLUE_BACKGROUND    = 1 << 4,
    GREEN_BACKGROUND   = 1 << 5,
    RED_BACKGROUND     = 1 << 6,
    INTENSE_BACKGROUND = 1 << 7
  };

  void init();

  //! \return The number of rows in the text graphics
  uint rows();
  uint cols();

  uint8_t text_attr();
  void set_text_attr(uint8_t);

  void clear();

  void put_char(int8_t c, uint position);
  void put_char(int8_t c, const point2d& position);

  int8_t char_at(uint position);
  int8_t char_at(const point2d& position);

  void set_cursor_pos(uint position);
  void set_cursor_pos(const point2d& position);
  void hide_cursor();
  uint cursor_pos();
}
