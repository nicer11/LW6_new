#ifndef HEADLINES_H
#define HEADLINES_H


#include <stdio.h>
#include <stdlib.h>

// Структура для хранения информации о книге
typedef struct {
    char author[50];
    char title[50];
    char year[50];
} Book;

// Объявления функций
void writeToFile(FILE *file, Book *books, int count);
void readFromFile(FILE *file);
void addRecord(FILE *file, int *count);
void deleteRecord(FILE *file, int *count);
void searchRecords(FILE *file);
void editBook(FILE *file);
void displayRecords(FILE *file);
void create_my_own(FILE *file, int *count);
void creation_by_user(FILE *file, int *count);
void printf_extend(const char* s, int length);
void safe_fgets(char* s, int size);
void menu();

#endif // HEADLINES_H