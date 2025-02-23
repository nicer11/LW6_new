#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <windows.h>
#include "headlines.h"

// Прототипы функций и указатели на функции
void (*searchRecords_ptr)(FILE *) = searchRecords;

int main() {
    // Установка кодировки для корректного отображения UTF-8 в консоли
    system("chcp 65001");
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    // Открытие файла
    // Разница между rb+ и wb+ в том, что для rb+ нужно, чтобы файл уже существовал, а для wb+ без разницы - он либо создат его, либо очистит существующий
    
    FILE *file = fopen("books.bin", "wb+");
    if (file == NULL) {
        printf("Ошибка при открытии файла.\n");
        return 1;
    }
    
    
    
    int count;
    #define MODE 1 // 1 - готовые данные, 2 - данные пользователя
    #if MODE == 1
    // Создание готовых данных и отображение записей
    printf("Констата MODE объявлена как 1, программа работает с готовыми данными\n");
    create_my_own(file, &count);
    displayRecords(file);
    #elif MODE == 2
        // Создание данных пользователем
        printf("Констата MODE объявлена как 2, программа работает с готовыми данными пользователя\n");
        creation_by_user(file, &count);
        displayRecords(file);
    #endif
    int choice;
    // Основной цикл программы
    do {
        // Отображение меню
        menu();
        
        
        scanf("%d", &choice);
        getchar(); // Удаление символа новой строки из буфера ввода

        // Выполнение действия в зависимости от выбора пользователя
        if (choice == 1) {
            // Добавление новой записи
            addRecord(file, &count);
            displayRecords(file);
        } else if (choice == 2) {
            // Удаление записи
            deleteRecord(file, &count);
            displayRecords(file);
        } else if (choice == 3) {
            // Поиск записей
            searchRecords_ptr(file);
        } else if (choice == 4) {
            // Редактирование записи
            editBook(file);
            displayRecords(file);
        } else if (choice == 5) {
            // Отображение всех записей
            displayRecords(file);
        }
    } while (choice != 6); // Завершение программы при выборе 6

    // Закрытие файла перед завершением программы
    fclose(file);

    return 0;
}