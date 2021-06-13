
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "basicOperations.h"

void append_strings(char * str1, char * str2)
{
printf("\nAqui: %s \n %s \n", str1, str2);
    int len1 = strlen(str1);
    int len2 = strlen(str2);

   /* Reallocating memory */
   str1 = (char *) realloc(str1, len1 + len2 +1);
    strcat(str1, str2);
    //str1[len2+len1+1] = '\0';
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

//---------------------- Read files --------------
ssize_t readln(int fd, char *line, size_t size) {
  ssize_t i = 0;
  ssize_t bytes_read;
  while (i < size && (bytes_read = read(fd, &line[i], 1)) > 0 &&
         line[i] != '\n')
    i++;
  line[i++] = '\n';
  return i;
}
