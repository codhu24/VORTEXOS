#include "keyboard.h"
#include "vesa.h"
#include "port.h"
#include "interrupts.h"
#include "pic.h"
static const char scancodes[] = {
    0,  0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,  0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const char scancodes_shift[] = {
    0,  0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,  0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

static int shift_pressed=0;
static int caps_lock=0;
static int control_pressed=0;
static int alt_pressed=0;
static char intput_buff[256];
static int buff_ind=0;
void keyboard_handler(struct interrupt_frame *frame)
{
uint8_t scancode =inb(KEYBOARD_DATA_PORT);
//handle press or release
if(scancode & 0x80)
{
    //key release
    scancode &= 0x7F;
    switch(scancode)
    {
        case 0x2A://left shift
        case 0x36://right shift
        shift_pressed=0;
        break;
        case 0x1D://control
        control_pressed=0;
        break;
        case 0x38:
        alt_pressed=0;
        break;
    }
}
else
{
   //key pressed
   switch(scancode)
   {
    case 0x2A:
    case 0x36:
    shift_pressed=1;
    break;
    case 0x3A:
    caps_lock=!caps_lock;
    break;
    case 0x1D:
    control_pressed=1;
    break;
    case 0x38:
    alt_pressed=1;
    break;
    case 0x0E:
    if(buff_ind>0)
    {
        buff_ind--;
        intput_buff[buff_ind]='\0';
    }
    break;
    case 0x1C://enter
    if(buff_ind<sizeof(intput_buff)-1)
    {
        intput_buff[buff_ind++]='\n';
        intput_buff[buff_ind]='\0';

    }
    break;
            default:
                if (scancode < sizeof(scancodes) && scancodes[scancode] != 0) {
                    char c;
                    if (shift_pressed || caps_lock) {
                        c = scancodes_shift[scancode];
                    } else {
                        c = scancodes[scancode];
                    }
                    
                    if (buff_ind< sizeof(intput_buff) - 1) {
                        intput_buff[buff_ind++] = c;
                        intput_buff[buff_ind] = '\0';
                    }
                }
                break;

   } 
}
if(scancode!=0){
    pic_send_eoi(1);
}

}
void init_keyboard()
{
buff_ind=0;
intput_buff[0]='\0';
//install keyboard intrrupt (IRQ 1)
irq_install_handler(1,keyboard_handler);
//enable keyboard irq
pic_clear_mask(1);
}

char keyboard_getchar()
{  //char c=0;//disable interrupts
    asm volatile("cli");
if(buff_ind>0)
{
    char c=intput_buff[0];
    for(int i=1;i<buff_ind;i++)
    {
          intput_buff[i-1]=intput_buff[i];
    }
    buff_ind--;
        asm volatile("sti");//renable interrrupts
    return c;
}
    asm volatile("sti");//renable interrupts

return 0;
}


int keyboard_has_input()
{
return buff_ind>0;
}
const char* keyboard_get_buffer()
{
return intput_buff;
}
void keyboard_clear_buffer()
{
    buff_ind=0;
    intput_buff[0]='\0';
}