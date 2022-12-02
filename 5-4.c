#include <stdio.h>

int strend(char* s, char* t) {
    int i;

    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] == t[0]) {
            // make new variable, so I can return to old value
            int j = i, k = 0;
            while (s[j++] == t[k++]) {
                if (s[j] == '\0' && t[k] == '\0') {
                    return 1;
                }
                else if (s[j] == '\0') {
                    break;
                }
                else if (t[k] == '\0') {
                    break;
                }
            }
        }
    }
    return 0;
}

int main(void) {
    char str[20] = "hello world";
    char str2[10] = "world";
    
    printf("%d", strend(str, str2));
}