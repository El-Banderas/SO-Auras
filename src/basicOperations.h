

#include <stdlib.h>

typedef struct {
  int *array;
  int used;
  int size;
} ArrayInt;

void initArrayInt(ArrayInt *a, int initialSize);
void insertArrayInt(ArrayInt *a, int element) ;
void freeArrayInt(ArrayInt *a);

typedef struct {
  char **array;
  int used;
  int size;
} ArrayChar;
void initArrayChar(ArrayChar *a, int initialSize);
void insertArrayChar(ArrayChar *a, char* element) ;
void freeArrayChar(ArrayChar *a);

void append_strings(char * str1, char *str2);
ssize_t readln(int fd, char *line, size_t size) ;