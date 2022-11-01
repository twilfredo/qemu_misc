#include "lib.rs.h"

volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;

void print_uart0(const char *s) {
    while(*s != '\0') { /* Loop until end of string */
        *UART0DR = (unsigned int)(*s); /* Transmit char */
        s++; /* Next char */
    }
}

long int cpp_hook_a() {
    print_uart0("cpp_callback_from_rust: [ok]\n");
    return 1313;
}

int test_build_token(void)
{
	long int index = 0;
    int expect = 0;
    int rc = 0;
	
	const uint8_t bufferCursor[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	rust::Slice<const uint8_t> slice{bufferCursor, 10};
	rc = rs_build_token_array(slice, index);

    for (unsigned int i = 0; i < sizeof(bufferCursor); ++i) {
        //Same logic performed by the rust function
        expect += bufferCursor[i];
    }

    if (expect == index) {
        print_uart0("test_build_token: array test [ok]\n");
    } else {
        print_uart0("test_build_token: array test [fail]\n");
    }

    return rc;
}

extern "C"
void c_entry() {

    print_uart0("\nSTART TEST\n");

    // hook_c returns it's arg * 10
    if (rs_hook_c(10) == 100) {
        print_uart0("hook_c assertion [ok]\n");
    } else {
        print_uart0("hook_c assertion [fail]\n");
    }
    
    // expected return value is -10
    if (test_build_token() == -10) {
        print_uart0("test_build_token [ok]\n");
    } else {
        print_uart0("test_build_token [fail]\n");
    }

    if (rs_ping_cpp() == true) {
        print_uart0("cpp_callback: magic num recvd [ok]\n");
    } else {
        print_uart0("cpp_callback: magic num inval [fail]\n");
    }
    print_uart0("END TEST\n");
}

// The rust::cxxbridge::Slice() function requires this, but no idea why
void __assert_func (const char *__a, int, const char *__b, const char *__c) {
	while (true) {}
}

// This is missing in a free-standing environment
// Purpose: this is used in the stack unwinding tables
// since we aren't using exceptions. Disable them with `-fno-exceptions`
// https://stackoverflow.com/questions/329059/what-is-gxx-personality-v0-for
void *__gxx_personality_v0;