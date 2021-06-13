
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "basicOperations.h"

char * append_strings(const char * old, const char * new)
{
    // find the size of the string to allocate
    printf("Passou \n");
    const size_t old_len = strlen(old), new_len = strlen(new);
    const size_t out_len = old_len + new_len + 1;

    // allocate a pointer to the new string
    char *out = malloc(out_len);

    // concat both strings and return
    memcpy(out, old, old_len);
    memcpy(out + old_len, new, new_len + 1);

    return out;
}

// ----------------------  Array of Ints ------------------------
void initArrayInt(ArrayInt *a, int initialSize) {
  a->array = (int *)malloc(initialSize * sizeof(int));
  a->used = 0;
  a->size = initialSize;
}

void insertArrayInt(ArrayInt *a, int element) {
  if (a->used == a->size) {
    a->size *= 2;
    a->array = (int *)realloc(a->array, a->size * sizeof(int));
  }
  a->array[a->used++] = element;
}
void freeArrayInt(ArrayInt *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

// ----------------------  Array of chars ------------------------
void initArrayChar(ArrayChar *a, int initialSize) {
  a->array = (char  **)malloc(initialSize * sizeof(char *));
  a->used = 0;
  a->size = initialSize;
}

void insertArrayChar(ArrayChar *a, char* element) {
  if (a->used == a->size) {
    a->size *= 2;
    a->array = (char **)realloc(a->array, a->size * sizeof(char *));
  }
  a->array[a->used++] = element;
}

void freeArrayChar(ArrayChar *a) {
  for (int i = 0; i < a->used; i++) free(a->array[i]);
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}
