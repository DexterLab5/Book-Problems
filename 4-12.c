#include <stdio.h>
#include <math.h>

void rec_itoa(int x, char* s, int i) {
    if (x < 0) {
        s[0] = '-';
        s += 1;
        x = -x;
    }
    
    if (i) {
        rec_itoa(x, s+1, i-1);
    }
    // get the last digit and put it in the last index
    s[0] = (x / (int)pow(10, i)) % 10 + '0';
}

int digits(int x) {
    int i = 0;
    while (x /= 10) i++;
    return i;
}

int main(void) {
    int num = -5556;
    char str[10];
    rec_itoa(num, str, digits(num));
    printf("%s", str);
}