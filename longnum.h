#ifndef LONGNUM_H
#define LONGNUM_H

typedef struct {
    int *digits;     // динамический массив для хранения цифр
    int capacity;    // текущая ёмкость массива
    int length;      // текущая длина числа
} LongNum;

// Добавим объявление глобальной переменной
extern char memory_messages[1000];

// Инициализация длинного числа
void initLongNum(LongNum *num);

// Освобождение памяти
void freeLongNum(LongNum *num);

// Увеличение размера массива при необходимости
void ensureCapacity(LongNum *num, int minCapacity);

// Умножение длинного числа на обычное целое число
void multiplyByInt(LongNum *num, int multiplier);

// Вывод длинного числа
void printLongNum(const LongNum *num);

// Подсчет количества битов в числе
int getBitCount(const LongNum *num);

#endif 