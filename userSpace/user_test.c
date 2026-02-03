#include "../include/userSpace/user_test.h"

int x = 0;

void main() {
	x++;
	int y = 0;
	while(1) {
		++y;
		asm("nop");
	}
}
