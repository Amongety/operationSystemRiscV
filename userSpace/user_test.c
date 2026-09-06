// #include "../include/userSpace/user_test.h"

static inline void callbackPrint(const char *fmt, unsigned long arg);

#define SIZE 1000
int myarray[SIZE];
int another_array[5] = {1, 2, 3, 4, 5};

int main() {
	callbackPrint("I'm a C program, and I'm running in user space. How about a big, Hello World\r\n", 0);
	
    callbackPrint("My array is at %x\r\n", (unsigned long)myarray);
	
    callbackPrint("I'm going to start crunching some numbers, so gimme a minute.\r\n", 0);
	
	
    for(int i = 0; i < SIZE; ++i) {
		myarray[i] = another_array[i % 5];
    }
	
    for(int i = 0; i < 100000000; ++i) {
		myarray[i % SIZE] += 1;
    }
	
	
    callbackPrint("Ok, I'm done crunching. Wanna see myarray[0]? It's %d\r\n", myarray[0]);
	
	return 0;
}

static inline void callbackPrint(const char *fmt, unsigned long arg) {
	register unsigned long a0 asm("a0") = (unsigned long)fmt;
	register unsigned long a1 asm("a1") = arg;
	register unsigned long a7 asm("a7") = 448;

	asm volatile(
		"ecall"
		: "+r"(a0)
		: "r"(a1),
		  "r"(a7)
		: "memory"
	);
}