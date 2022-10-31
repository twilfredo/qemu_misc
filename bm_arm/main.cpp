#include "lib.rs.h"

volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;

void print_uart0(const char *s) {
    while(*s != '\0') { /* Loop until end of string */
        *UART0DR = (unsigned int)(*s); /* Transmit char */
        s++; /* Next char */
    }
}

void test_build_token()
{
	long int WorkIndex = 0;
	
	const uint8_t BufferCursor[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	rust::Slice<const uint8_t> slice{BufferCursor, 10};
	//int StatusCode = build_token_array(slice, WorkIndex);
}

extern "C"
void c_entry() {
    print_uart0("START\n");
    if (hook_c(10) == 10*10) {
        print_uart0("hook_c assertion [ok]\n");
    }
    //test_build_token();
}

// The rust::cxxbridge::Slice() function requires this, but no idea why
void __assert_func (const char *__a, int, const char *__b, const char *__c) {
	while (true) {}
}