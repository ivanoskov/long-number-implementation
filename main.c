#include "longnum.h"
#include <unistd.h>  // для sleep()
#include <stdio.h>  // для sprintf() и strcat()

void initLongNum(LongNum *num) {
    memset(num->digits, 0, sizeof(num->digits));
    num->digits[0] = 1;
    num->length = 1;
}

void multiplyByInt(LongNum *num, int multiplier) {
    long long carry = 0;
    
    for (int i = 0; i < num->length || carry; i++) {
        long long current = carry + (long long)num->digits[i] * multiplier;
        num->digits[i] = current % BASE;
        carry = current / BASE;
        if (i >= num->length && carry)
            num->length = i + 1;
    }
}

void printLongNum(const LongNum *num) {
    printf("%d", num->digits[num->length - 1]);
    for (int i = num->length - 2; i >= 0; i--) {
        printf("%09d", num->digits[i]);
    }
    printf("\n");
}

int getBitCount(const LongNum *num) {
    int totalBits = 0;
    int lastNonZero = num->length - 1;
    
    // Подсчет битов в последнем ненулевом элементе
    int lastNum = num->digits[lastNonZero];
    while (lastNum > 0) {
        totalBits++;
        lastNum >>= 1;
    }
    
    // Добавляем биты от остальных элементов
    totalBits += lastNonZero * 30; // каждый элемент содержит 30 бит (BASE = 10^9)
    
    return totalBits;
}

// Функция для очистки консоли
void clearScreen() {
    printf("\033[H\033[J");  // ANSI escape sequence для очистки экрана
}

// Функция для отрисовки прогресс-бара
void drawProgressBar(int current, int total) {
    int barWidth = 50;
    float progress = (float)current / total;
    int pos = barWidth * progress;

    printf("[");
    for (int i = 0; i < barWidth; i++) {
        if (i < pos) printf("█");
        else if (i == pos) printf(">");
        else printf(" ");
    }
    printf("] %d%%\r", (int)(progress * 100));
    fflush(stdout);
}

int main() {
    int n;
    printf("\033[1;36m"); // Голубой цвет
    printf("╔════════════════════════════════════╗\n");
    printf("║    Калькулятор овербольших чисел   ║\n");
    printf("╚════════════════════════════════════╝\n");
    printf("\033[0m"); // Сброс цвета

    printf("\nВведите число для подсчета факториала: ");
    scanf("%d", &n);
    
    LongNum result;
    initLongNum(&result);
    
    printf("\n\033[1;33m"); // Желтый цвет
    printf("Начинаем вычисление %d!\n", n);
    printf("\033[0m"); // Сброс цвета

    int milestones[] = {8, 16, 32, 64};
    int milestoneNames[] = {0, 0, 0, 0}; // флаги для отслеживания пройденных рубежей
    
    // Массив для хранения истории сообщений
    char history[1000] = "";
    
    for (int i = 2; i <= n; i++) {
        clearScreen();
        
        printf("\033[1;36m"); // Голубой цвет
        printf("Вычисление факториала %d\n", n);
        printf("\033[0m"); // Сброс цвета
        
        drawProgressBar(i - 1, n);
        printf("\n\n");

        // Выводим накопленную историю
        printf("%s", history);

        multiplyByInt(&result, i);
        int bits = getBitCount(&result);

        // Проверяем новые рубежи
        for (int j = 0; j < 4; j++) {
            if (bits > milestones[j] && !milestoneNames[j]) {
                char milestone_msg[100];
                sprintf(milestone_msg, "\033[1;32m✓ Преодолен рубеж %d бит ", milestones[j]);
                strcat(history, milestone_msg);
                
                switch(milestones[j]) {
                    case 8: strcat(history, "(char)\033[0m\n"); break;
                    case 16: strcat(history, "(short)\033[0m\n"); break;
                    case 32: strcat(history, "(int)\033[0m\n"); break;
                    case 64: strcat(history, "(long long)\033[0m\n"); break;
                }
                milestoneNames[j] = 1;
            }
        }

        printf("\nТекущий размер: %d бит\n", bits);
        usleep(50000); // Небольшая задержка для визуального эффекта
    }

    clearScreen();
    printf("\033[1;32m"); // Зеленый цвет
    printf("╔════════════════════════════════════╗\n");
    printf("║        Вычисление завершено!       ║\n");
    printf("╚════════════════════════════════════╝\n");
    printf("\033[0m"); // Сброс цвета

    printf("\nИстория преодоления рубежей:\n");
    printf("%s\n", history);

    printf("\nРезультат %d! = ", n);
    printLongNum(&result);
    
    printf("\nИтоговый размер числа: %d бит\n", getBitCount(&result));
    
    return 0;
} 