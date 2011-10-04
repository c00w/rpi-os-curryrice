#include "kernel.h"

namespace kernel
{
  bool strcmp(char * first, char * second){
    while(first && second){
      if (*first != *second){
        return false;
      }
      first++; second++;
    }
    return true;

  }

  void proccess(char * values){
    char * a = "quit";
    if (strcmp(values, a)){
      return -1;
    }

    return 1;
  }

  void main()
  {
    //Buffer for storing commands
    char string_buffer[200];
    int pos = 0;
    int buff_index = 0;
    
    //Zero out the buffer
    for( int i = 0; i < 200; i++){
      string_buffer[i] = char(0);
    }

    //Loop until we wan to quit
    char key = 0;
    char escape = 0;
    while (escape != -1){

      key = keyboard::get_char();

      //Check if we are inputing data
      if (key != '\n'){

        //Store it
        string_buffer[buff_index] = key;
        buff_index++;

        //Move characters and echo it.
        text_graphics::put_char(key, pos);
        pos++;
        text_graphics::set_cursor_pos(pos);

      } else{

        //Go to the newline and set position + proccess command
        pos = (pos/80)*80 + 80;

        //Clear and reset if we go to far.
        if (pos > 23*80){
            text_graphics::clear();
            pos = 0;
        }
        text_graphics::set_cursor_pos(pos);
        if(proccess(string_buffer) == -1){
            escape = -1;
        };
      }
    }
  }
}
