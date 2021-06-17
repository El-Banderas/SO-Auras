
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "basicOperations.h"


// ----------------------  Array of Ints ------------------------
ArrayInt *initArrayInt(int initialSize) {
    ArrayInt *a = malloc(sizeof(ArrayInt));
    a = malloc(sizeof(ArrayInt));
    a->array = (int *) malloc(initialSize * sizeof(int));
    a->used = 0;
    a->size = initialSize;
    return a;
}

int getArrayInt(ArrayInt *a, int pos) {
    //printf("X == //  %d <  %d\n",   a->used, pos);
    if (a->used < pos) return -1;
    //printf("GetArrayChar %s \n", a->array[-1 + (a->used)]);
    if (!a->array[pos]) return -1;
    return a->array[pos];
}

void changeArrayInt(ArrayInt *a, int pos, int toSum) {
    //printf("X == //  %d <  %d\n",   a->used, pos);
    if (a->used < pos) return;
    //printf("GetArrayChar %s \n", a->array[-1 + (a->used)]);
     a->array[pos] = toSum+ a->array[pos];
}

void insertArrayInt(ArrayInt *a, int element) {
    if (a->used == a->size) {
        a->size *= 2;
        a->array = (int *) realloc(a->array, a->size * sizeof(int));
    }
    a->array[a->used++] = element;
}



void freeArrayInt(ArrayInt *a) {
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

ArrayInt * duplicateArrayInt(ArrayInt * a){
    ArrayInt *res = initArrayInt(a->used);
   for (int i = 0; i < a->used; i++) insertArrayInt(res, a->array[i]);
   return res; 
}

// ----------------------  Array of chars ------------------------
ArrayChar *initArrayChar(int initialSize) {
    ArrayChar *a = malloc(sizeof(ArrayChar));
    a->array = (char **) malloc(initialSize * sizeof(char *));
    a->used = 0;
    a->size = initialSize;
    return a;
}

void insertArrayChar(ArrayChar *a, char *element) {
    //O problema está aqui
    if (a->used == a->size) {
        a->size *= 2;
        a->array = (char **) realloc(a->array, a->size * sizeof(char *));
    }
    int size = strlen(element);
    char *x = malloc(sizeof(char) * size);
    strcpy(x, element);
    a->array[a->used++] = x;
    //printf("Inserted %s \n", a->array[-1 + (a->used)]);
}

char *getArrayChar(ArrayChar *a, int pos) {
    //printf("X == //  %d <  %d\n",   a->used, pos);
    if (a->used < pos) return "Invalid";
    //printf("GetArrayChar %s \n", a->array[-1 + (a->used)]);
    if (a->array[pos] == NULL) return "Invalid";
    int size = strlen(a->array[pos]);
    char *x = malloc(sizeof(char) * size);
    strcpy(x, a->array[pos]);
    return x;
}

int getSize(ArrayChar a) {
    return a.used;
}

void freeArrayChar(ArrayChar *a) {
    for (int i = 0; i < a->used; i++) free(a->array[i]);
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

ArrayChar * duplicateArrayChar(ArrayChar * a){
    ArrayChar *res = initArrayChar(a->used);
   for (int i = 0; i < a->used; i++) insertArrayChar(res, strdup(a->array[i]));
   return res; 
}
//---------------------- Read files --------------
ssize_t readln(int fd, char *line, size_t size) {
    ssize_t i = 0;
    ssize_t bytes_read;
    while (i < size && (bytes_read = read(fd, &line[i], 1)) && line[i] != '\n') {
        i++;
    }
    line[i++] = '\n';
    return i;
}

