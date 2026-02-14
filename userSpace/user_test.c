#include "../include/userSpace/user_test.h"

int x = 0;

int main() {
	x++;
	asm("addi sp, sp, -16");
	int y = 0;
	++y;
	asm("nop");
	asm("addi sp, sp, 16");

	return 0;
}
