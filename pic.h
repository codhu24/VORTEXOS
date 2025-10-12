//pic.h
//Declares the functions that other parts of the OS can use to interact with the PIC
#ifndef PIC_H
#define PIC_H

#include<stdint.h>

void pic_remap();
void pic_send_eoi(uint8_t irq);
void pic_set_mask(uint8_t irq);
void pic_clear_mask(uint8_t irq);
#endif