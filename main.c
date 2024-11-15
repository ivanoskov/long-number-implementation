#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "longnum.h"

char memory_messages[1000] = "";

void clearScreen() {
    printf("\033[H\033[J");
}

void drawProgressBar(int current, int total) {
    int barWidth = 50;
    float progress = (float)current / total;
    int pos = barWidth * progress;

    printf("[");
    for (int i = 0; (i < barWidth); i++) {
        if (i < pos) {
            printf("█");
        } else if (i == pos) {
            printf(">");
        } else {
            printf(" ");
        }
    }
    printf("] %d%%\r", (int)(progress * 100));
    fflush(stdout);
}

void appendToHistory(char *history, const char *message) {
    strcat(history, message);
}

int main() {
    int n;
    printf("\033[1;36m");
    printf("╔════════════════════════════════════╗\n");
    printf("║     Калькулятор факториала         ║\n");
    printf("╚════════════════════════════════════╝\n");
    printf("\033[0m");

    printf("\nВведите число для подсчета факториала: ");
    if (scanf("%d", &n) != 1) {
        printf("Ошибка при чтении числа!\n");
        return 1;
    }
    
    LongNum result;
    char history[2000] = "";
    
    // Добавляем первое сообщение о памяти в историю
    initLongNum(&result);
    strcat(history, memory_messages);
    memory_messages[0] = '\0';  // Очищаем буфер сообщений о памяти
    
    printf("\n\033[1;33m");
    printf("Начинаем вычисление %d!\n", n);
    printf("\033[0m");

    int milestones[] = {8, 16, 32, 64};
    int milestoneNames[] = {0, 0, 0, 0};
    
    for (int i = 2; (i <= n); i++) {
        clearScreen();
        
        printf("\033[1;36m");
        printf("Вычисление факториала %d\n", n);
        printf("\033[0m");
        
        drawProgressBar(i - 1, n);
        printf("\n\n");
        printf("%s", history);

        multiplyByInt(&result, i);
        
        // Добавляем новые сообщения о памяти в историю
        if (strlen(memory_messages) > 0) {
            strcat(history, memory_messages);
            memory_messages[0] = '\0';
        }
        
        int bits = getBitCount(&result);

        for (int j = 0; (j < 4); j++) {
            if ((bits > milestones[j]) && (!milestoneNames[j])) {
                char milestone_msg[200];
                switch(milestones[j]) {
                    case 8:
                        sprintf(milestone_msg, "\033[1;32m✓ Преодолен рубеж %d бит (char)\033[0m\n", milestones[j]);
                        break;
                    case 16:
                        sprintf(milestone_msg, "\033[1;32m✓ Преодолен рубеж %d бит (short)\033[0m\n", milestones[j]);
                        break;
                    case 32:
                        sprintf(milestone_msg, "\033[1;32m✓ Преодолен рубеж %d бит (int)\033[0m\n", milestones[j]);
                        break;
                    case 64:
                        sprintf(milestone_msg, "\033[1;32m✓ Преодолен рубеж %d бит (long long)\033[0m\n", milestones[j]);
                        break;
                }
                appendToHistory(history, milestone_msg);
                milestoneNames[j] = 1;
            }
        }

        printf("\nТекущий размер: %d бит\n", bits);
        usleep(50000);
    }

    clearScreen();
    printf("\033[1;32m");
    printf("╔════════════════════════════════════╗\n");
    printf("║        Вычисление завершено!       ║\n");
    printf("╚════════════════════════════════════╝\n");
    printf("\033[0m");

    printf("\nИстория изменений:\n");
    printf("%s\n", history);

    printf("\nРезультат %d! = ", n);
    printLongNum(&result);
    
    printf("\nИтоговый размер числа: %d бит\n", getBitCount(&result));
    printf("\033[1;35mОсвобождение памяти...\033[0m\n");
    freeLongNum(&result);
    
    return 0;
} 