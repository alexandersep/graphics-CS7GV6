#include "graphics.h"

void cg_tool_itoa(char* str, int n) {
    while (n > 0) {
        int remainder = n % 10;        
        arrput(str, remainder + '0');
        n /= 10; 
    }
}

int cg_tool_count_bits(int n) {
    int bits = 0;
    while (n > 0) {
        bits += n & 1;
        n >>= 1;
    }
    return bits;
}
