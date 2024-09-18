#include "graphics.h"

void cg_tool_itoa(char* str, int n) {
    while (n != 0) {
        int remainder = n % 10;        
        arrput(str, remainder + '0');
        n /= 10; 
    }
}
