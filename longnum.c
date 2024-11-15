#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "longnum.h"

#define BASE 1000000000
#define INITIAL_CAPACITY 16

void initLongNum(LongNum *num) {
    num->capacity = INITIAL_CAPACITY;
    num->digits = (int*)calloc(num->capacity, sizeof(int));
    num->digits[0] = 1;
    num->length = 1;
    
    char message[200];
    sprintf(message, "\033[1;35mВыделено памяти: %zu байт\033[0m",
            num->capacity * sizeof(int));
    
    extern char memory_messages[1000];
    strcat(memory_messages, message);
    strcat(memory_messages, "\n");
}

void freeLongNum(LongNum *num) {
    free(num->digits);
    num->digits = NULL;
    num->capacity = 0;
    num->length = 0;
}

void ensureCapacity(LongNum *num, int minCapacity) {
    if (minCapacity > num->capacity) {
        int newCapacity = num->capacity * 2;
        if (newCapacity < minCapacity) {
            newCapacity = minCapacity;
        }
        
        int *newDigits = (int*)realloc(num->digits, newCapacity * sizeof(int));
        if (newDigits == NULL) {
            printf("\033[1;31mОшибка выделения памяти!\033[0m\n");
            exit(1);
        }
        
        char message[200];
        sprintf(message, "\033[1;35mУвеличение буфера (в блоках): %d -> %d (%zu байт)\033[0m",
                num->capacity, newCapacity, newCapacity * sizeof(int));
        
        extern char memory_messages[1000];
        strcat(memory_messages, message);
        strcat(memory_messages, "\n");
        
        num->digits = newDigits;
        num->capacity = newCapacity;
    }
}

void multiplyByInt(LongNum *num, int multiplier) {
    long long carry = 0;
    
    for (int i = 0; ((i < num->length) || (carry)); i++) {
        if (i >= num->capacity) {
            ensureCapacity(num, i + 1);
        }
        
        long long current = carry;
        if (i < num->length) {
            current += (long long)num->digits[i] * multiplier;
        }
        
        num->digits[i] = current % BASE;
        carry = current / BASE;
        
        if ((i >= num->length) && ((num->digits[i] != 0) || (carry))) {
            num->length = i + 1;
        }
    }
}

void printLongNum(const LongNum *num) {
    printf("%d", num->digits[num->length - 1]);
    for (int i = (num->length - 2); (i >= 0); i--) {
        printf("%09d", num->digits[i]);
    }
    printf("\n");
}

int getBitCount(const LongNum *num) {
    int totalBits = 0;
    int lastNonZero = num->length - 1;
    
    int lastNum = num->digits[lastNonZero];
    while (lastNum > 0) {
        totalBits++;
        lastNum >>= 1;
    }
    
    totalBits += lastNonZero * 30;
    
    return totalBits;
} 