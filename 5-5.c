#include <stdio.h>

void strncpy(char* s, char* t, int n) {
    while (n-- && *t) {
        *s++ = *t++;
    }
}

void strncat(char* s, char* t, int n) {
    // Move s to the end if not empty
    if (*s) {
        while (*++s);
    }
    // Insert n chars from t
    while ((*s++ = *t++) && --n);
}

int strncmp2(char* s, char* t, int n) {
    // compare untill n is 2 or s doesn't equal t
    for (; *s == *t && n > 1; s++, t++, n--)
        if (*s == '\0')
            return 0;
    return *s - *t;
}

int main(void) {
    char s[20] = "hello";
    char s2[10] = "helwo";
    int x = strncmp2(s, s2, 3);
    printf("%d", x);
}