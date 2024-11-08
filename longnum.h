#ifndef LONGNUM_H
#define LONGNUM_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BASE 1000000000
#define MAX_DIGITS 100000

typedef struct {
    int digits[MAX_DIGITS];  // массив для хранения цифр числа
    int length;              // текущая длина числа
} LongNum;

// Инициализация длинного числа
void initLongNum(LongNum *num);

// Умножение длинного числа на обычное целое число
void multiplyByInt(LongNum *num, int multiplier);

// Вывод длинного числа
void printLongNum(const LongNum *num);

// Подсчет количества битов в числе
int getBitCount(const LongNum *num);

#endif 