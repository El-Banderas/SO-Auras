

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

ArrayInt * duplicateArrayInt(ArrayInt * a);

typedef struct {
    char **array;
    int used;
    int size;
} ArrayChar; 

ArrayChar *initArrayChar(int initialSize);

void insertArrayChar(ArrayChar *a, char *element);

void freeArrayChar(ArrayChar *a);

ssize_t readln(int fd, char *line, size_t size);

char *getArrayChar(ArrayChar *a, int pos);

int getSize(ArrayChar a);

ArrayChar * duplicateArrayChar(ArrayChar * a);