#include <xc.h>

extern long mul_extended(int n, int m);

void main(void) {
    volatile long ans = mul_extended(-32768,32767);
    while(1);
}