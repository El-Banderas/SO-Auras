

#include <stdlib.h>

typedef struct {
    int *array;
    int used;
    int size;
} ArrayInt;

void initArrayInt(ArrayInt *a, int initialSize);

void insertArrayInt(ArrayInt *a, int element);

void freeArrayInt(ArrayInt *a);

typedef struct {
    char **array;
    int used;
    int size;
} ArrayChar; // METE COM * DIOGOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

void initArrayChar(ArrayChar *a, int initialSize);

void insertArrayChar(ArrayChar *a, char *element);

void freeArrayChar(ArrayChar *a);

ssize_t readln(int fd, char *line, size_t size);

char *getArrayChar(ArrayChar *a, int pos);

int getSize(ArrayChar a);
