#include "pic.h"
#include "x86.h"

namespace x86
{
  namespace pic
  {
    //
    // PIC 1 register port addresses
    //
    #define I86_PIC1_REG_COMMAND 0x20
    #define I86_PIC1_REG_STATUS  0x20
    #define I86_PIC1_REG_DATA    0x21
    #define I86_PIC1_REG_IMR     0x21

    //
    // PIC 2 register port addresses
    //
    #define I86_PIC2_REG_COMMAND 0xA0
    #define I86_PIC2_REG_STATUS  0xA0
    #define I86_PIC2_REG_DATA    0xA1
    #define I86_PIC2_REG_IMR     0xA1

    //
    // Initialization Control Word 1 bit masks
    //
    #define I86_PIC_ICW1_MASK_IC4  0x1  //00000001
    #define I86_PIC_ICW1_MASK_SNGL 0x2  //00000010
    #define I86_PIC_ICW1_MASK_ADI  0x4  //00000100
    #define I86_PIC_ICW1_MASK_LTIM 0x8  //00001000
    #define I86_PIC_ICW1_MASK_INIT 0x10 //00010000

    // Initialization Control Words 2 and 3 do not require bit masks

    //
    // Initialization Control Word 4 bit masks
    //
    #define I86_PIC_ICW4_MASK_UPM  0x1  //00000001
    #define I86_PIC_ICW4_MASK_AEOI 0x2  //00000010
    #define I86_PIC_ICW4_MASK_MS   0x4  //00000100
    #define I86_PIC_ICW4_MASK_BUF  0x8  //00001000
    #define I86_PIC_ICW4_MASK_SFNM 0x10 //00010000

    //
    // Initialization Command 1 control bits
    //
    #define I86_PIC_ICW1_IC4_EXPECT          1    //1
    #define I86_PIC_ICW1_IC4_NO              0    //0
    #define I86_PIC_ICW1_SNGL_YES            2    //10
    #define I86_PIC_ICW1_SNGL_NO             0    //00
    #define I86_PIC_ICW1_ADI_CALLINTERVAL4   4    //100
    #define I86_PIC_ICW1_ADI_CALLINTERVAL8   0    //000
    #define I86_PIC_ICW1_LTIM_LEVELTRIGGERED 8    //1000
    #define I86_PIC_ICW1_LTIM_EDGETRIGGERED  0    //0000
    #define I86_PIC_ICW1_INIT_YES            0x10 //10000
    #define I86_PIC_ICW1_INIT_NO             0    //00000

    //
    // Initialization Command 4 control bits
    //
    #define I86_PIC_ICW4_UPM_86MODE	   1    //1
    #define I86_PIC_ICW4_UPM_MCSMODE     0    //0
    #define I86_PIC_ICW4_AEOI_AUTOEOI    2    //10
    #define I86_PIC_ICW4_AEOI_NOAUTOEOI  0    //0
    #define I86_PIC_ICW4_MS_BUFFERMASTER 4    //100
    #define I86_PIC_ICW4_MS_BUFFERSLAVE  0    //0
    #define I86_PIC_ICW4_BUF_MODEYES     8    //1000
    #define I86_PIC_ICW4_BUF_MODENO	   0    //0
    #define I86_PIC_ICW4_SFNM_NESTEDMODE 0x10 //10000
    #define I86_PIC_ICW4_SFNM_NOTNESTED  0    //0

    //
    // PIC 1 IRQs
    //
    #define I86_PIC_IRQ_TIMER     0
    #define I86_PIC_IRQ_KEYBOARD  1
    #define I86_PIC_IRQ_SERIAL2   3
    #define I86_PIC_IRQ_SERIAL1   4
    #define I86_PIC_IRQ_PARALLEL2 5
    #define I86_PIC_IRQ_DISKETTE  6
    #define I86_PIC_IRQ_PARALLEL1 7

    //
    // PIC 2 IRQs
    //
    #define I86_PIC_IRQ_CMOSTIMER  0
    #define I86_PIC_IRQ_CGARETRACE 1
    #define I86_PIC_IRQ_AUXILIARY  4
    #define I86_PIC_IRQ_FPU        5
    #define I86_PIC_IRQ_HDC        6

    //
    // Command Word 2 bit masks
    //
    #define I86_PIC_OCW2_MASK_L1     1    //00000001
    #define I86_PIC_OCW2_MASK_L2     2    //00000010
    #define I86_PIC_OCW2_MASK_L3     4    //00000100
    #define I86_PIC_OCW2_MASK_EOI    0x20 //00100000
    #define I86_PIC_OCW2_MASK_SL     0x40 //01000000
    #define I86_PIC_OCW2_MASK_ROTATE 0x80 //10000000

    //
    // Command Word 3 bit masks
    //
    #define I86_PIC_OCW3_MASK_RIS  1    //00000001
    #define I86_PIC_OCW3_MASK_RIR  2    //00000010
    #define I86_PIC_OCW3_MASK_MODE 4    //00000100
    #define I86_PIC_OCW3_MASK_SMM  0x20 //00100000
    #define I86_PIC_OCW3_MASK_ESMM 0x40 //01000000
    #define I86_PIC_OCW3_MASK_D7   0x80 //10000000

    void init()
    {
      // Disable hardware interrupts
      CLI();

      // Begin initialization of PIC
      OUTB(I86_PIC1_REG_COMMAND, I86_PIC_ICW1_INIT_YES | I86_PIC_ICW1_IC4_EXPECT);
      IOWait();
      OUTB(I86_PIC2_REG_COMMAND, I86_PIC_ICW1_INIT_YES | I86_PIC_ICW1_IC4_EXPECT);
      IOWait();

      // Send initialization control word 2. This is the base addresses of the irq's
      OUTB(I86_PIC1_REG_DATA, 32);
      IOWait();
      OUTB(I86_PIC2_REG_DATA, 40);
      IOWait();

      // Send initialization control word 3. This is the connection between master and slave.
      // ICW3 for master PIC is the IR that connects to secondary pic in binary format
      // ICW3 for secondary PIC is the IR that connects to master pic in decimal format
      OUTB(I86_PIC1_REG_DATA, 4);
      IOWait();
      OUTB(I86_PIC2_REG_DATA, 2);
      IOWait();

      // Send Initialization control word 4. Enables i86 mode
      OUTB(I86_PIC1_REG_DATA, I86_PIC_ICW4_UPM_86MODE);
      IOWait();
      OUTB(I86_PIC2_REG_DATA, I86_PIC_ICW4_UPM_86MODE);
      IOWait();

      STI();

      // Enable keyboard interrupt
      OUTB(I86_PIC1_REG_DATA, ~2);
      OUTB(I86_PIC2_REG_DATA, 0);
    }

    void signal_eoi(uint irq)
    {
      if(irq >= 8) // Test if we need to send end-of-interrupt to second pic
	OUTB(I86_PIC2_REG_COMMAND, I86_PIC_OCW2_MASK_EOI);

      // Always send end-of-interrupt to primary pic
      OUTB(I86_PIC1_REG_COMMAND, I86_PIC_OCW2_MASK_EOI);
    }
  }
}
