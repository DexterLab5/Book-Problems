#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *calloc2(size_t items, size_t size) {
    if (items == 0 || size == 0)
        return NULL;
    
    void *mem = malloc(items * size);
    /* clear the memory */
    memset(mem, 0x00, items * size);
    return mem;
}

void printarr(int*);

int main(void) {
    int *arr = calloc2(5, sizeof(int));
    printarr(arr);
    
    for (int i = 0; i < 5; i++)
        arr[i] = i + 1;
    
    printarr(arr);
}

void printarr(int *arr) {
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
