Here is a detailed, file-by-file and function-by-function breakdown of the user-defined and predefined C variables used in your operating system project.

### Classification of Variables

* **User-Defined Variables**: These are variables, structures, and arrays explicitly declared by you to store data and manage the state of the OS. This includes global variables, local variables within functions, and function parameters.
* **Predefined C Types**: These are types defined in standard C headers like `stdint.h` and `stddef.h`. While not built into the C language itself, they are standard and provide fixed-width integers, which are crucial for OS development. Examples: `uint8_t`, `uint32_t`, `size_t`.
* **Built-in C Types**: These are the fundamental data types provided by the C language itself. Examples: `int`, `char`, `void*`.

---

### **File: `newos/gdt.c`**

This file initializes the Global Descriptor Table (GDT), which defines memory segments for the kernel.

* #### **Global Variables**
    * `struct gdt_entry gdt[GDT_ENTRIES]`: **User-Defined**. An array of `gdt_entry` structures that holds the GDT itself. `GDT_ENTRIES` is a macro defined as 6.
    * `struct gdt_ptr gp`: **User-Defined**. A structure that holds the size and base address of the `gdt` array, which is then loaded into the CPU's GDTR register.

* #### **Function: `gdt_set_gate`**
    * **Parameters**:
        * `int32_t num`: **User-Defined** (Predefined type `int32_t`). The index in the `gdt` array to configure.
        * `uint32_t base`: **User-Defined** (Predefined type `uint32_t`). The 32-bit base address of the memory segment.
        * `uint32_t limit`: **User-Defined** (Predefined type `uint32_t`). The 32-bit size limit of the segment.
        * `uint8_t access`: **User-Defined** (Predefined type `uint8_t`). The access byte, defining permissions and type.
        * `uint8_t gran`: **User-Defined** (Predefined type `uint8_t`). The granularity byte.
    * **Local Variables**: None.

* #### **Function: `init_gdt`**
    * **Parameters**: None.
    * **Local Variables**: None.

---

### **File: `newos/interrupts.c`**

This file sets up the Interrupt Descriptor Table (IDT) and the main handlers for CPU exceptions and hardware interrupts.

* #### **Global Variables**
    * `struct idt_gate idt[IDT_ENTRIES]`: **User-Defined**. An array of 256 `idt_gate` structures, representing the entire IDT.
    * `struct idt_ptr idtp`: **User-Defined**. A structure holding the limit and base address of the `idt` array.
    * `void (*irq_handlers[16])(struct interrupt_frame *)`: **User-Defined**. An array of 16 function pointers. Each element can point to a specific driver's handler for a hardware interrupt (IRQ).

* #### **Function: `pic_clear_irq`**
    * **Parameters**:
        * `uint8_t irq`: **User-Defined** (Predefined type `uint8_t`). The IRQ number to clear the mask for.
    * **Local Variables**: None.

* #### **Function: `set_idt_gate`**
    * **Parameters**:
        * `uint8_t num`: **User-Defined** (Predefined type `uint8_t`). The index (0-255) of the interrupt in the IDT.
        * `uint32_t base`: **User-Defined** (Predefined type `uint32_t`). The address of the interrupt handler function.
        * `uint16_t sel`: **User-Defined** (Predefined type `uint16_t`). The code segment selector for the handler.
        * `uint8_t flags`: **User-Defined** (Predefined type `uint8_t`). Flags defining the type and attributes of the gate.
    * **Local Variables**: None.

* #### **Function: `init_idt`**
    * **Parameters**: None.
    * **Local Variables**:
        * `void (*isrs[])()`: **User-Defined**. An array of function pointers initialized with the addresses of the low-level assembly ISR stubs (`isr0`, `isr1`, etc.).
        * `int i`: **User-Defined** (Built-in C type). A loop counter.
        * `void (*irqs[])()`: **User-Defined**. An array of function pointers for the low-level IRQ stubs (`irq0`, `irq1`, etc.).

* #### **Function: `load_tss`**
    * **Parameters**: None.
    * **Local Variables**: None.

* #### **Function: `isr_handler`**
    * **Parameters**:
        * `struct interrupt_frame *frame`: **User-Defined**. A pointer to the stack frame pushed by the assembly stub, containing saved CPU registers.
    * **Local Variables**: None.

* #### **Function: `irq_handler`**
    * **Parameters**:
        * `struct interrupt_frame *frame`: **User-Defined**. A pointer to the stack frame.
    * **Local Variables**:
        * `uint8_t irq`: **User-Defined** (Predefined type `uint8_t`). The calculated IRQ number (interrupt number - 32).

* #### **Function: `irq_install_handler`**
    * **Parameters**:
        * `int irq`: **User-Defined** (Built-in C type). The IRQ number (0-15) to install a handler for.
        * `void (*handler)(struct interrupt_frame *)`: **User-Defined**. A function pointer to the new handler routine.
    * **Local Variables**: None.

* #### **Function: `irq_uninstall_handler`**
    * **Parameters**:
        * `int irq`: **User-Defined** (Built-in C type). The IRQ number whose handler should be removed.
    * **Local Variables**: None.

---

### **File: `newos/kernel.c`**

This is the main entry point of the C kernel, responsible for initialization and the main application loop.

* #### **Function: `draw_dashboard`**
    * **Parameters**: None.
    * **Local Variables**:
        * `int i`, `int j`, `int k`: **User-Defined** (Built-in C type). Loop counters for drawing icons.
        * `int x`: **User-Defined** (Built-in C type). The x-coordinate for drawing placeholder icons.

* #### **Function: `write_string`**
    * **Parameters**:
        * `int color`: **User-Defined** (Built-in C type). A color attribute for the text (legacy text mode).
        * `const char* str`: **User-Defined** (Built-in C type). A pointer to the null-terminated string to be written.
    * **Local Variables**:
        * `volatile char* video_memory`: **User-Defined** (Built-in C type). A pointer to the legacy text mode video memory address `0xB8000`.

* #### **Function: `kernel_main`**
    * **Parameters**:
        * `uint32_t magic`: **User-Defined** (Predefined type `uint32_t`). The magic number passed by the Multiboot loader to verify it.
        * `uint32_t mb_info`: **User-Defined** (Predefined type `uint32_t`). The memory address of the Multiboot information structure.
    * **Local Variables**:
        * `int i`: **User-Defined** (Built-in C type). A loop counter used to incorrectly set PIC masks.
        * `int y_pos`, `int x_pos`: **User-Defined** (Built-in C type). Coordinates for drawing, though unused in the final loop.
        * `char c`: **User-Defined** (Built-in C type). Stores the character read from the keyboard.

---

### **File: `newos/keyboard.c`**

This file is the device driver for the keyboard, handling scancodes and providing character input.

* #### **Global (static) Variables**
    * `static const char scancodes[]`: **User-Defined**. An array mapping keyboard scancodes to standard ASCII characters. `static` limits its scope to this file.
    * `static const char scancodes_shift[]`: **User-Defined**. An array mapping scancodes to shifted ASCII characters (e.g., '1' -> '!').
    * `static int shift_pressed`, `caps_lock`, `control_pressed`, `alt_pressed`: **User-Defined**. Flags to track the state of modifier keys.
    * `static char intput_buff[256]`: **User-Defined**. A circular buffer to store incoming keyboard characters.
    * `static int buff_ind`: **User-Defined**. The current index/size of the keyboard buffer.

* #### **Function: `keyboard_handler`**
    * **Parameters**:
        * `struct interrupt_frame *frame`: **User-Defined**. Pointer to the interrupt frame, unused here but required by the IRQ handler signature.
    * **Local Variables**:
        * `uint8_t scancode`: **User-Defined** (Predefined type `uint8_t`). Stores the raw scancode read from the keyboard port `0x60`.
        * `char c`: **User-Defined** (Built-in C type). Stores the final ASCII character after processing modifier keys.

* #### **Function: `init_keyboard`**
    * **Parameters**: None.
    * **Local Variables**: None.

* #### **Function: `keyboard_getchar`**
    * **Parameters**: None.
    * **Local Variables**:
        * `char c`: **User-Defined** (Built-in C type). The character to be returned from the buffer.
        * `int i`: **User-Defined** (Built-in C type). A loop counter to shift the buffer contents down.

* #### **Functions: `keyboard_has_input`, `keyboard_get_buffer`, `keyboard_clear_buffer`**
    * **Parameters**: None.
    * **Local Variables**: None.

---

### **File: `newos/pic.c`**

This file controls the 8259 Programmable Interrupt Controller (PIC), which manages hardware interrupts.

* #### **Function: `pic_remap`**
    * **Parameters**: None.
    * **Local Variables**:
        * `uint8_t a1`, `uint8_t a2`: **User-Defined** (Predefined type `uint8_t`). Used to save the original masks of the master and slave PICs before remapping.

* #### **Function: `pic_send_eoi`**
    * **Parameters**:
        * `uint8_t irq`: **User-Defined** (Predefined type `uint8_t`). The IRQ number that has just been serviced.
    * **Local Variables**: None.

* #### **Function: `pic_set_mask` / `pic_clear_mask`**
    * **Parameters**:
        * `uint8_t irq`: **User-Defined** (Predefined type `uint8_t`). The IRQ line to enable or disable.
    * **Local Variables**:
        * `uint16_t port`: **User-Defined** (Predefined type `uint16_t`). The data port address of the master or slave PIC.
        * `uint8_t value`: **User-Defined** (Predefined type `uint8_t`). The new mask value to be written to the port.

---

### **File: `newos/shell.c`**

This file implements a simple command-line interpreter (shell).

* #### **Global (static) Variables**
    * `static char cmd_buffer[MAX_COMMAND_LENGTH]`: **User-Defined**. A buffer to store the command currently being typed by the user.
    * `static int cursor_pos`: **User-Defined**. The current character position within `cmd_buffer`.
    * `static char cmd_his[MAX_HISTORY][MAX_COMMAND_LENGTH]`: **User-Defined**. A 2D array to store a history of executed commands.
    * `static int hist_cnt`: **User-Defined**. The total number of commands stored in the history.
    * `static int hist_ind`: **User-Defined**. The current position when navigating history (not fully implemented).
    * `static int current_line`: **User-Defined**. Tracks the current line on the screen to print output to, creating a scrolling effect.

* #### **Function: `init_shell`**
    * **Parameters**: None.
    * **Local Variables**: None.

* #### **Function: `print_prompt`**
    * **Parameters**: None.
    * **Local Variables**: None.

* #### **Function: `execute_command`**
    * **Parameters**:
        * `char *input`: **User-Defined** (Built-in C type). The full command string entered by the user.
    * **Local Variables**:
        * `char *args[MAX_ARGS]`: **User-Defined**. An array of character pointers, where each pointer points to a token (argument) in the command string.
        * `int arg_count`: **User-Defined**. The number of arguments found.
        * `char input_copy[MAX_COMMAND_LENGTH]`: **User-Defined**. A mutable copy of the input string, as tokenizing modifies the string.
        * `char *token`: **User-Defined**. A pointer used to iterate through `input_copy` during tokenization.
        * `int i`: **User-Defined**. A loop counter.
        * `char output[MAX_COMMAND_LENGTH]`: **User-Defined**. A buffer to construct the output for the `echo` command.
        * `char error_msg[64]`: **User-Defined**. A buffer to construct the "Unknown command" error message.

* #### **Function: `draw_shell_icon`**
    * **Parameters**:
        * `int x`, `int y`: **User-Defined** (Built-in C type). The top-left coordinates to draw the icon.
    * **Local Variables**:
        * `uint32_t icon_color`, `bg_color`: **User-Defined** (Predefined type `uint32_t`). The colors for the icon's graphics.
        * `int i`, `int j`: **User-Defined**. Loop counters.

* #### **Function: `shell_loop`**
    * **Parameters**: None.
    * **Local Variables**:
        * `char c`: **User-Defined**. Stores the character read from the keyboard.
        * `char debug_msg[64]`: **User-Defined**. A buffer used to display debugging information on screen.
        * `char hex[8]`, `uint8_t val`, `char nib1`, `char nib2`: **User-Defined**. Variables used to convert a character's byte value to a hexadecimal string for debugging.
        * `char char_str[2]`: **User-Defined**. A temporary string to hold a single printable character for debugging messages.

---

### **File: `newos/string.c`**

This file provides custom implementations of standard C string library functions.

* #### **Function: `mystrlen`**
    * **Parameters**:
        * `const char *str`: **User-Defined**. A pointer to a constant, null-terminated string.
    * **Local Variables**:
        * `size_t len`: **User-Defined** (Predefined type `size_t`). A counter to store the length.

* #### **Function: `mystrcmp`**
    * **Parameters**:
        * `const char *s1`, `const char *s2`: **User-Defined**. Pointers to the two strings to be compared.
    * **Local Variables**: None.

* #### **Function: `mystrcpy`**
    * **Parameters**:
        * `char *dest`: **User-Defined**. A pointer to the destination buffer.
        * `const char *src`: **User-Defined**. A pointer to the source string.
    * **Local Variables**:
        * `char *ptr`: **User-Defined**. A temporary pointer used to traverse the destination buffer.

* #### **Function: `mystrcat`**
    * **Parameters**:
        * `char *dest`: **User-Defined**. The destination string to append to.
        * `const char *src`: **User-Defined**. The source string to be appended.
    * **Local Variables**:
        * `char *ptr`: **User-Defined**. A pointer used to find the end of the `dest` string.

* #### **Function: `mymemset`**
    * **Parameters**:
        * `void *ptr`: **User-Defined**. A generic pointer to the block of memory to fill.
        * `int value`: **User-Defined**. The value (as an int, but cast to a char) to fill the memory with.
        * `size_t num`: **User-Defined**. The number of bytes to fill.
    * **Local Variables**:
        * `unsigned char* p`: **User-Defined**. A pointer used to iterate byte-by-byte through the memory block.

* #### **Function: `mymemcpy`**
    * **Parameters**:
        * `void *dest`, `const void *src`: **User-Defined**. Pointers to the destination and source memory areas.
        * `size_t n`: **User-Defined**. The number of bytes to copy.
    * **Local Variables**:
        * `unsigned char* d`: **User-Defined**. A character pointer to the destination.
        * `const unsigned char* s`: **User-Defined**. A character pointer to the source.

---

### **File: `newos/tss.c`**

This file initializes the Task State Segment (TSS), used here to define the kernel stack for interrupt handling.

* #### **Global Variables**
    * `struct tss_entry tss`: **User-Defined**. The instance of the TSS structure itself.

* #### **Function: `init_tss`**
    * **Parameters**: None.
    * **Local Variables**:
        * `uint32_t base`: **User-Defined**. The base address of the `tss` global variable.
        * `uint32_t limit`: **User-Defined**. The size of the `tss` structure.
        * `uint8_t *ptr`: **User-Defined**. A pointer used to iterate through the `tss` structure to zero it out.
        * `uint32_t i`: **User-Defined**. A loop counter.

---

### **File: `newos/vesa.c`**

This is the driver for VESA graphics mode, handling all screen drawing operations.

* #### **Global Variables**
    * `struct framebuffer fb`: **User-Defined**. A structure containing all relevant information about the graphics mode (width, height, pitch, address).

* #### **Global (static) Variables**
    * `static uint8_t current_font_size`: **User-Defined**. The current scaling factor for the font (default is 1).
    * `static const uint8_t font[128][8]`: **User-Defined**. The bitmap data for an 8x8 pixel font for ASCII characters 0-127.

* #### **Function: `write_string1`**
    * **Parameters**: `int color`, `const char* str`.
    * **Local Variables**: `volatile char* video_memory`.
    * (Note: This is a legacy text-mode function, likely for early debugging, distinct from the VESA drawing functions.)

* #### **Functions: `get_font_size`, `get_char_width`, `get_char_height`**
    * **Parameters**: None.
    * **Local Variables**: None.

* #### **Function: `set_pixel`**
    * **Parameters**: `uint32_t x`, `uint32_t y`, `uint32_t color`.
    * **Local Variables**: `uint32_t *pixel`: **User-Defined**. A pointer calculated to point to the exact 32-bit memory location for the desired pixel.

* #### **Function: `fill_screen`**
    * **Parameters**: `uint32_t color`.
    * **Local Variables**:
        * `uint32_t total_pixels`: **User-Defined**. The total number of pixels on the screen.
        * `uint32_t *pixel`: **User-Defined**. A pointer to the start of the framebuffer memory.
        * `uint32_t i`: **User-Defined**. A loop counter.

* #### **Function: `init_vesa`**
    * **Parameters**: `uint32_t magic`, `uint32_t mb_info_ptr`.
    * **Local Variables**:
        * `struct multiboot_info *mbi`: **User-Defined**. A pointer to the Multiboot information structure, which contains the framebuffer details provided by GRUB.

* #### **Function: `draw_char` / `draw_string`**
    * **Parameters**: `uint32_t x`, `uint32_t y`, `char c` or `const char* str`, `uint32_t color`.
    * **Local Variables**: None.

* #### **Function: `draw_char_scaled`**
    * **Parameters**: `uint32_t x`, `uint32_t y`, `char c`, `uint32_t color`, `uint8_t scale`.
    * **Local Variables**:
        * `unsigned char uc`: **User-Defined**. The character cast to an unsigned type for use as an array index.
        * `uint32_t cy`, `cx`: **User-Defined**. Loop counters for iterating through the font bitmap rows and columns.
        * `uint32_t sy`, `sx`: **User-Defined**. Loop counters for scaling the pixel drawing.

* #### **Function: `fill_rect`**
    * **Parameters**: `uint32_t x`, `y`, `width`, `height`, `color`.
    * **Local Variables**: `uint32_t dy`, `dx`: **User-Defined**. Loop counters.

* #### **Function: `draw_string_scaled`**
    * **Parameters**: `uint32_t x`, `y`, `const char* str`, `color`, `uint8_t scale`.
    * **Local Variables**:
        * `uint32_t start_x`: **User-Defined**. The initial x-position to return to on a new line.
        * `uint32_t cx`, `cy`: **User-Defined**. The current x and y coordinates for drawing.
        * `uint32_t char_width`, `char_height`: **User-Defined**. The calculated width and height of a single scaled character.