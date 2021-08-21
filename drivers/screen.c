#include "screen.h"
#include "../kernel/low_level.c"

int get_screen_offset(int col, int row);
int get_cursor();
void set_cursor(int offset);
int handle_scrolling(int cursor_offset);

void print_char(char character, int col, int row, char attribute_byte) {
	//Pointer to begin video memory
	unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
	
	//if attribute zero, assume white_on_black
	if(!attribute_byte) {
		attribute_byte = WHITE_ON_BLACK;
	}
	
	int offset;
	
	//if column and row non negative, use those for char position
	if(col >= 0 && row >= 0) {
		offset = get_screen_offset(col, row);
	//otherwise, current cursor position
	} else {
		offset = get_cursor();
	}
	
	//if newline, set offset to end of row
	if (character == "\n") {
		//offset is amount of bytes from 0xb8000, 
		//row is zero if smaller than MAX_COLS, so rows 
		//smaller than one so also zero
		int rows = offset / (2 * MAX_COLS);
		offset = get_screen_offset(79, rows);
	} else {
		vidmem[offset] = character;
		vidmem[offset + 1] = attribute;
	}
	
	//for moving the cursor
	offset += 2;
	//for when reaching bottom of the screen
	offset = handle_scrolling(offset);
	
	set_cursor(offset);
}

int get_screen_offset(int col, int row) {
	return (row * MAX_COLS + col) * 2;
}

int get_cursor() {
	//change the content of the data register to that of 
	//internal register 14, which is the high byte of the position.
	port_byte_out(REG_SCREEN_CTRL, 14);
	offset = port_byte_in(REG_SCREEN_DATA) << 8;
	port_byte_out(REG_SCREEN_CTRL, 15);
	offset += port_byte_in(REG_SCREEN_DATA);
	
	return offset * 2; //because amount of characters is stored instead of 
					   //amount of bytes
}

void set_cursor(int offset) {
	//from number of bytes to number of chars
	offset /= 2;
	//changing things, so port_byte_in is not needed
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
	port_byte_out(REG_SCREEN_CTRL, 15);
	
	//don't get why this does not have to be a char
	port_byte_out(REG_SCREEN_DATA, offset);
}

void handle_scrolling(int cursor_offset) {
	//if cursor is still on screen
	if (cursor_offset < MAX_COLS * MAX_ROWS * 2) {
		return cursor_offset;
	} 
	
	//TODO: the rest of this function
}

void print_at(char* message, int col, int row) {
	if (col >= 0 && row >= 0){
		set_cursor(get_screen_offset(col, row)); 
	}
	
	int i = 0; 
	//This means there is a zero at the end of the string
	for (i = 0; i < strlen(message); i++) {
		print_char(message[i], col, row, WHITE_ON_BLACK);
	}
}

void print(char* message) {
	print_at(message, -1, -1);
}

void clear_screen() {
	int row = 0;
	int col = 0; 
	
	for(row = 0; row < MAX_ROWS; row++) {
		for(col = 0; col < MAX_COLS; col++) {
			print_char(' ', col, row, WHITE_ON_BLACK);
		}
	}
	
	set_cursor(0);
}