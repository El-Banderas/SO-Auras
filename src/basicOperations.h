

#include <stdlib.h>

typedef struct {
    int *array;
    int used;
    int size;
} ArrayInt;

ArrayInt *initArrayInt(int initialSize);

void insertArrayInt(ArrayInt *a, int element);

int getArrayInt(ArrayInt *a, int pos);

void freeArrayInt(ArrayInt *a);

void changeArrayInt(ArrayInt *a, int pos, int toSum);

typedef struct {
    char **array;
    int used;
    int size;
} ArrayChar; // METE COM * DIOGOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

ArrayChar *initArrayChar(int initialSize);

void insertArrayChar(ArrayChar *a, char *element);

void freeArrayChar(ArrayChar *a);

ssize_t readln(int fd, char *line, size_t size);

char *getArrayChar(ArrayChar *a, int pos);

int getSize(ArrayChar a);
