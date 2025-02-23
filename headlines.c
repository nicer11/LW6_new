#include "headlines.h"
#include <stdio.h>
#include <string.h>
#include <io.h>



// Реализация функции safe_fgets
void safe_fgets(char* s, int size) {
    if (fgets(s, size, stdin)) {
        size_t len = strlen(s);
        if (len > 0 && s[len - 1] == '\n') {
            s[len - 1] = '\0';
        }
    }
}

void menu() {
    printf("Меню:\n");
    printf("1. Добавить запись\n");
    printf("2. Удалить запись\n");
    printf("3. Поиск записей\n");
    printf("4. Редактировать запись\n");
    printf("5. Вывести все записи\n");
    printf("6. Выйти\n");
    printf("Ваш выбор: ");
}

void printf_extend(const char* s, int length) {
    int true_len = 0;
    int i = 0;
    
    while (s[i] != '\0') {
        if ((s[i] & 0xC0) != 0x80) {
            true_len++;
        }
        i++;
    }
    
    printf("%s", s);
    for (int j = 0; j < length - true_len; j++) {
        printf(" ");
    }
}
// Функция для записи массива книг в бинарный файл
void writeToFile(FILE *file, Book *books, int count) {
    fseek(file, 0, SEEK_SET);
    fwrite(books, sizeof(Book), count, file);
    fflush(file);
}

// Функция для чтения массива книг из бинарного файла
void readFromFile(FILE *file) {
    fseek(file, 0, SEEK_SET);
    Book book;
    int index = 0;
    while (fread(&book, sizeof(Book), 1, file)) {
        printf("Индекс: %d, Автор: %s, Название: %s, Год: %s\n", index++, book.author, book.title, book.year);
    }
}

// Функция для редактирования информации о книге по индексу
void editBook(FILE *file) {
    int index;
    printf("Введите индекс записи для редактирования: ");
    scanf("%d", &index);
    getchar();
    fseek(file, (index - 1) * sizeof(Book), SEEK_SET);

    Book book;
    // Ввод новой информации о книге
    printf("Введите нового автора книги: ");
    safe_fgets(book.author, sizeof(book.author));
    printf("Введите новое название книги: ");
    safe_fgets(book.title, sizeof(book.title));
    printf("Введите новый год издания книги: ");
    safe_fgets(book.year, sizeof(book.year));

    fwrite(&book, sizeof(Book), 1, file);
    fflush(file);
    
}

// Функция для добавления записи в файл
void addRecord(FILE *file, int *count) {
    fseek(file, 0, SEEK_END);
    Book book;
    printf("Введите автора: ");
    safe_fgets(book.author, sizeof(book.author));
    printf("Введите название: ");
    safe_fgets(book.title, sizeof(book.title));
    printf("Введите год: ");
    safe_fgets(book.year, sizeof(book.year));
    fwrite(&book, sizeof(Book), 1, file);
    fflush(file);
    (*count)++;
}

// Функция для удаления записи из файла по индексу
void deleteRecord(FILE *file, int *count) {
    int index;
    printf("Введите индекс записи для удаления: ");
    scanf("%d", &index);
    getchar();
    index--;

    if (index < 0 || index >= (*count)) {
        printf("Неверный индекс.\n");
        return;
    }

    Book book;
    for (int i = index + 1; i < (*count); i++) {
        fseek(file, i * sizeof(Book), SEEK_SET);
        fread(&book, sizeof(Book), 1, file);
        fseek(file, (i - 1) * sizeof(Book), SEEK_SET);
        fwrite(&book, sizeof(Book), 1, file);
    }

    // Обрезка файла до нового размера
    _chsize_s(_fileno(file), ((*count) - 1) * sizeof(Book)); // _fileno(file) - Дескриптор файла может быть использован для выполнения низкоуровневых операций ввода-вывода, таких как изменение размера файла с помощью _chsize_s
    fflush(file);
    (*count)--;
}

// Функция для поиска записей по заданному полю
void searchRecords(FILE *file) {
    char value[50];
    int field;
    printf("Введите критерий для поиска (число 1 = автор, число 2 = название, число 3 = год): ");
    scanf("%d", &field);
    getchar();
    printf("Введите значение для поиска: ");
    safe_fgets(value, sizeof(value));
    
    int found = 0;
    int num = 1;
    printf("Индекс\tАвтор\t\t\tНазвание\t\t   Год\n");
    printf("-------------------------------------------------------------\n");
    fseek(file, 0, SEEK_SET);
    Book book;
    if (field == 1) {
        while (fread(&book, sizeof(Book), 1, file)) {
            if (strcmp(book.author, value) == 0) {
                printf("%d\t", num);
                printf_extend(book.author, 24);
                printf_extend(book.title, 27);
                printf("%s\n", book.year);
                found = 1;
            }
            num++;
        }
    } else if (field == 2) {
        while (fread(&book, sizeof(Book), 1, file)) {
            if (strcmp(book.title, value) == 0) {
                printf("%d\t", num);
                printf_extend(book.author, 24);
                printf_extend(book.title, 26);
                printf("%s\n", book.year);
                found = 1;
            }
            num++;
        }
    } else if (field == 3) {
        while (fread(&book, sizeof(Book), 1, file)) {
            if (strcmp(book.year, value) == 0) {
                printf("%d\t", num);
                printf_extend(book.author, 24);
                printf_extend(book.title, 26);
                printf("%s\n", book.year);
                found = 1;
            }
            num++;
        }
    } else {
        printf("Неверное поле для поиска.\n");
    }

    if (!found) {
        printf("Записи не найдены (нет точного совпадения).\n");
    }
}

// Функция для вывода содержимого файла в табличном виде
void displayRecords(FILE *file) {
    printf("Индекс\tАвтор\t\t\tНазвание\t\t   Год\n");
    printf("-------------------------------------------------------------\n");
    int index = 0;
    fseek(file, 0, SEEK_SET);
    Book book;
    while (fread(&book, sizeof(Book), 1, file)) {
        printf("%d\t", index + 1);
        printf_extend(book.author, 24);
        printf_extend(book.title, 27);
        printf("%s\n", book.year);
        index++;
    }
}

// Функция для создания массива книг с готовыми данными
void create_my_own(FILE *file, int *count) {
    Book books[] = {
        {"Александр Пушкин", "Евгений Онегин", "1823"}, // Год тоже в формате строки,чтобы у пользователя было больше свободы, чтобы он мог записывать в этот пункт любые символы: вдруг ему захочется 
        {"Лев Толстой", "Война и мир", "1869"},
        {"Федор Достоевский", "Преступление и наказание", "1866"},
        {"Николай Гоголь", "Мертвые души", "1842"},
        {"Иван Тургенев", "Отцы и дети", "1862"},
        {"Антон Чехов", "Вишневый сад", "1904"},
        {"Михаил Булгаков", "Мастер и Маргарита", "1967"},
        {"Александр Солженицын", "Архипелаг ГУЛАГ", "1973"},
        {"Александр Пушкин", "Капитанская дочка", "1836"},
        {"Александр Пушкин", "Борис Годунов", "1825"},
        {"Лев Толстой", "Анна Каренина", "1877"},
        {"Лев Толстой", "Детство", "1852"},
        {"Федор Достоевский", "Идиот", "1869"},
        {"Федор Достоевский", "Братья Карамазовы", "1880"},
        {"Николай Гоголь", "Ревизор", "1836"},
        {"Николай Гоголь", "Тарас Бульба", "1835"},
        {"Иван Тургенев", "Дворянское гнездо", "1859"},
        {"Иван Тургенев", "Записки охотника", "1852"},
        {"Антон Чехов", "Чайка", "1896"},
        {"Антон Чехов", "Дядя Ваня", "1899"},
        {"Михаил Булгаков", "Белая гвардия", "1925"},
        {"Михаил Булгаков", "Собачье сердце", "1925"},
        {"Александр Солженицын", "Раковый корпус", "1968"},
        {"Александр Солженицын", "Один день Ивана Денисовича", "1962"},
        {"Александр Пушкин", "Руслан и Людмила", "1820"},
        {"Лев Толстой", "Крейцерова соната", "1889"},
        {"Федор Достоевский", "Бесы", "1872"},
        {"Николай Гоголь", "Майская ночь", "1832"},
        {"Иван Тургенев", "Ася", "1858"},
        {"Антон Чехов", "Толстый и тонкий", "1883"},
        {"Михаил Булгаков", "Дьяволиада", "1925"},
        {"Александр Солженицын", "Октябрьская трилогия", "1974"},
        {"Александр Пушкин", "Пиковая дама", "1834"},
        {"Лев Толстой", "Смерть Ивана Ильича", "1886"},
        {"Федор Достоевский", "Игрок", "1866"},
        {"Николай Гоголь", "Вий", "1835"},
        {"Иван Тургенев", "Накануне", "1860"},
        {"Антон Чехов", "В овраге", "1900"},
        {"Михаил Булгаков", "Багровый остров", "1927"},
        {"Александр Солженицын", "Канцлер", "2000"},
        {"Александр Пушкин", "Медный всадник", "1837"},
        {"Лев Толстой", "Хаджи-Мурат", "1912"},
        {"Федор Достоевский", "Белые ночи", "1848"},
        {"Николай Гоголь", "Шинель", "1842"}
    };
    *count = sizeof(books) / sizeof(Book);
    fseek(file, 0, SEEK_SET);
    fwrite(books, sizeof(Book), *count, file);
    fflush(file);
    
}

// Функция для создания массива книг с вводом данных пользователем
void creation_by_user(FILE *file, int *count) {
    printf("Введите количество книг: ");
    scanf("%d", count);
    getchar();
    for (int i = 0; i < (*count); i++) {
        Book book;
        printf("Введите автора %d: ", i + 1);
        safe_fgets(book.author, sizeof(book.author));
        printf("Введите название %d: ", i + 1);
        safe_fgets(book.title, sizeof(book.title));
        printf("Введите год %d: ", i + 1);
        safe_fgets(book.year, sizeof(book.year));
        fwrite(&book, sizeof(Book), 1, file);
    }
    fflush(file);
   
}