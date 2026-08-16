#include <xc.h>

extern unsigned char is_prime(unsigned char n);

void main(void) {
    volatile unsigned char ans = is_prime(253);
    while(1);
}