unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);
int get_screen_offset(int col, int row);
int get_cursor();
void set_cursor(int offset);
int handle_scrolling(int cursor_offset);
void print_at(char* message, int col, int row);
void print(char* message);
void clear_screen();

void main() {
	unsigned char *vidmem = (unsigned char *) 0xb8000;
	int i = 0; 
	int offset = 0;
	int col = 0; 
	int row = 0;
	
	
	for (row=0; row<25; row++) {
        for (col=0; col<80; col++) {
			*vidmem = ' ';
			vidmem += 1;
			*vidmem = 15;
			vidmem += 1;
        }
    }
	
	vidmem = 0xb8000;
	char* boodschap = "Boodschap\0";
	while(1) {
		if(*boodschap == 0) {
			break;
		}
		*vidmem = *boodschap;
		vidmem += 2;
		boodschap += 1;
	}
	
	vidmem = 0xb8000;
	*vidmem = 'F';
}