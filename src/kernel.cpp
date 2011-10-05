#include "kernel.h"

namespace kernel
{

    int lenstr(const char * in){
        int l = 0;
        while(*in != 0){
            in++;
            l++;
        }
        return l;

    };

    class String{
        public:
            String(const char * in){
                this->char_pointer = in;
                this->len = lenstr(in);
            }
        private:
            const char * char_pointer;
            int len;
    };

    class Console{
        public:
            Console(){    
                //Zero out the buffer
                for( int i = 0; i < 200; i++){
                  this->string_buffer[i] = char(0);
                }
                this->pos = 0;
                this->buff_index = 0;
            };

            int proccess(const char * c){return 1;};
            void write(String str);
            bool one_step(){
                  char key = keyboard::get_char();

                  //Check if we are inputing data
                  if (key != '\n'){

                    //Store it
                    this->string_buffer[this->buff_index] = key;
                    this->buff_index++;

                    //Move characters and echo it.
                    text_graphics::put_char(key, this->pos);
                    this->pos++;
                    text_graphics::set_cursor_pos(pos);

                  } else{

                    //Go to the newline and set position + proccess command
                    int cols = text_graphics::cols();
                    this->pos = (this->pos/cols)*cols + cols;

                    //Clear and reset if we go to far.
                    if (this->pos > 23*cols){
                        text_graphics::clear();
                        this->pos = 0;
                    }
                    text_graphics::set_cursor_pos(this->pos);
                    if(proccess(this->string_buffer) == -1){
                       return false;
                    };
                } 
                return true;
            }

        private:
            //Buffer for storing commands
            char string_buffer[200];
            int pos;
            int buff_index;

    };

bool strcmp(const char * first, const char * second){
    while(first && second){
      if (*first != *second){
        return false;
      };
      first++; second++;
    };
    return true;
  }

  void main()
  {

    //Loop until we wan to quit
    char key = 0;
    char escape = 0;
    Console console = Console();
    while (console.one_step()){

      
    }
  }
}
