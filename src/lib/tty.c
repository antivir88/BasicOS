#include "stdarg.h"
#include "stdlib.h"
#include "interrupt.h"
#include "tty.h"


typedef struct { uint8 chr; uint8 attr; } TtyChar;

unsigned int tty_width;
unsigned int tty_height;
unsigned int cursor;
uint8 text_attr;
TtyChar *tty_buffer;
uint16 tty_io_port;


void keyboard_int_handler();


void init_tty()
{
    tty_buffer = (void*)0xB8000;
    tty_width = *((uint16*)0x44A);
    tty_height = 25;
    tty_io_port = *((uint16*)0x463);
    cursor = (*((uint8*)0x451)) * tty_width + (*((uint8*)0x450));
    text_attr = 7;
    //set_int_handler(irq_base + 1, keyboard_int_handler, 0x8E);
}

void out_char(char chr)
{
    switch (chr) {
        case '\n':
            move_cursor((cursor / tty_width + 1) * tty_width);
            break;
        default:
            tty_buffer[cursor].chr = chr;
            tty_buffer[cursor].attr = text_attr;
            move_cursor(cursor + 1);
    }
}

void out_string(char *str)
{
    while (*str) {
        out_char(*str);
        str++;
    }
}

void clear_screen()
{
    memset_word(tty_buffer, (text_attr << 8) + ' ', tty_width * tty_height);
    move_cursor(0);
}

void set_text_attr(char attr)
{
    text_attr = attr;
}

//tty_io_port

void move_cursor(unsigned int pos)
{
    cursor = pos;
    if (cursor >= tty_width * tty_height) {
        cursor = (tty_height - 1) * tty_width;
        memcpy(tty_buffer, tty_buffer + tty_width, tty_width * tty_height * sizeof(TtyChar));
        memset_word(tty_buffer + tty_width * (tty_height - 1), (text_attr << 8) + ' ', tty_width);
    }

    /*outb(tty_io_port, 0x0E);
    outb(tty_io_port + 1, cursor >> 8);
    outb(tty_io_port, 0x0F);
    outb(tty_io_port + 1, cursor & 0xFF);*/
}

/*
void write_text(char* text, short color) {
    char* vga = (void*)0xb8000;

    for (int i = 0; i < strlen(text); ++i) {
        vga[i + 80] = color | text[i];
    }
}

void clear_screen2() {
    char* vga = (void*)0xb8000;
    for (int i = 0; i < 80 * 25 * 2; i++) {
        vga[i] = 0;
    }
}*/

const char digits[] = "0123456789ABCDEF";
char num_buffer[65];

char* int_to_str(size_t value, unsigned char base)
{
    size_t i = sizeof(num_buffer) - 1;
    num_buffer[i--] = '\0';

    do {
        num_buffer[i--] = digits[value % base];
        value = value / base;
    } while (value);

    return &num_buffer[i + 1];
}

void printf(char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            size_t arg = va_arg(args, size_t);

            switch (*fmt) {
                case '%':
                    out_char('%');
                    break;
                case 'c':
                    out_char(arg);
                    break;
                case 's':
                    out_string((char*)arg);
                    break;
                case 'b':
                    out_string(int_to_str(arg, 2));
                    break;
                case 'o':
                    out_string(int_to_str(arg, 8));
                    break;
                case 'd':
                    out_string(int_to_str(arg, 10));
                    break;
                case 'x':
                    out_string(int_to_str(arg, 16));
                    break;
            }
        } else {
            out_char(*fmt);
        }

        fmt++;
    }
    va_end(args);
}

IRQ_HANDLER(keyboard_int_handler) {
    uint8 key_code;

    key_code = inb(0x60);
    printf("You pressed key with code %d\n", key_code);

    uint8 status;

    status = inb(0x61);
    status |= 1;
    outb(0x61, status);
} 
