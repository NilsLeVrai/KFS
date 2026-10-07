#include "../../includes/stdint.h"

#define VGA_ADRESS 0xB8000
#define VGA_ROW 25
#define VGA_COL 80
#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

uint16_t *vga_adr_ptr = (uint16_t *)VGA_ADRESS;

typedef enum __attribute__ ((__packed__)) e_vga_color {
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GREY = 7,
	DARK_GREY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_MAGENTA = 13,
	YELLOW = 14,
	WHITE = 15
} vga_color_t;


unsigned char port_byte_in(unsigned short port) {
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

void set_cursor(int offset) {
    offset /= 2;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char) (offset >> 8));
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char) (offset & 0xff));
}

int get_cursor() {
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = port_byte_in(VGA_DATA_REGISTER) << 8;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset += port_byte_in(VGA_DATA_REGISTER);
    return offset * 2;
}

static inline uint8_t create_vga_color(uint8_t fg, uint8_t bg) {
	return (fg | bg << 4);
}

static inline uint16_t create_vga_char(uint8_t c, uint8_t color) {
	return ((uint16_t)c | (uint16_t)color << 8);
}

void clear_term() {
	int count = 0;
	for (int i = 0; i < VGA_COL; i++) {
		for (int j = 0; j < VGA_ROW; j++) {
			*(vga_adr_ptr + count) = create_vga_char('x', create_vga_color((vga_color_t)BLACK, (vga_color_t)BLACK));
			count ++;
		}
	}
	set_cursor(0);
}

void	print_char_at(uint8_t c, uint32_t x, uint32_t y, uint8_t fg, uint8_t bg) {
	*(vga_adr_ptr + ((VGA_COL * y) + x)) = create_vga_char(c, create_vga_color(fg, bg));
}

uint32_t	print_str_at(char* str, uint32_t x, uint32_t y, uint8_t fg, uint8_t bg) {
	uint32_t	xoffset = 0;
	uint32_t	yoffset = 0;
	uint32_t	newline_count = 0;
	while (*str) {
		if (x + xoffset >= VGA_COL) {
			x = 0;
			xoffset = 0;
			newline_count++;
		}
		if (*str == '\n') {
			str++;
			x = 0;
			xoffset = 0;
			newline_count++;
			continue ;
		}
		print_char_at(*str, x + xoffset, y + newline_count, fg, bg);
		xoffset++;
		str++;
	}
	return (VGA_COL * newline_count) + xoffset - 1;
}

void init_term() {
	clear_term();
}

void kernel_main(void) {
	init_term();
	uint32_t offset = print_str_at("eulmanOS >> ", 0, 0, (vga_color_t)MAGENTA, (vga_color_t)GREEN);
	set_cursor(offset * 2);
}
