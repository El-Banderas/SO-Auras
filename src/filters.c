#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include "request.h"
#include "filters.h"

struct Filters {
    ArrayChar *filtersNames; // Há diferença entre o path e o outro
    ArrayChar *filtersPath; // Name of filters
    ArrayInt *availableFilters;
    ArrayInt *maxFilters;
};

struct Filters *filters = NULL;


int filtersMissing(ArrayChar *requestFilters) {
    for (int i = 0; i < getSize(*(filters->filtersNames)); i++) {
        for (int j = 0; j < getSize(*(requestFilters)); j++) {
            if (!strcmp(getArrayChar((filters->filtersNames), i), getArrayChar(requestFilters, j))
                && getArrayInt(filters->availableFilters, i) == 0)
                return 1;
        }

    }
    return 0;
}

void initFilterStructur() {
    filters = malloc(sizeof(struct Filters));
    filters->filtersNames = initArrayChar(2);
    filters->filtersPath = initArrayChar(2);


    filters->availableFilters = initArrayInt(2);
    filters->maxFilters = initArrayInt(2);
}

void addFilter(char *filter) {
    char name[100];
    char path[100];
    int available;
    //printf("AddFilter: %s \n", filter, strlen(filter));
    sscanf(filter, "%s %s %d", name, path, &available);
    // printf("Name %s\nPath %s\n Number %d\n ", name, path, available);
    insertArrayChar(filters->filtersNames, name);
    insertArrayChar(filters->filtersPath, path);
    insertArrayInt(filters->availableFilters, available);
    insertArrayInt(filters->maxFilters, available);
//    printf("%d %s\n", 0, getArrayChar(&(current->filtersNames) , 0));
}

// @Override
ArrayChar *toString() {
    ArrayChar *toString = initArrayChar(2);
    insertArrayChar(toString, "Aqui inserem-se os requestes\n");

    for (int i = 0; i < getSize(*(filters->filtersNames)); i++) {
        char buffer[300];
        int total = getArrayInt((filters->maxFilters), i);
        int running = total - getArrayInt((filters->availableFilters), i);
        sprintf(buffer, "Filter %s: %d / %d (runing/max)\n", getArrayChar((filters->filtersNames), i), running, total);
        insertArrayChar(toString, buffer);
        /*
        strcat(buffer, getArrayChar(filters->filtersNames));
        strcat(buffer, ": ");
        */
        //       printf("To String %d %s\n", i, getArrayChar(&(filters->filtersNames), i));
    }
    //for (int i = 0; i < getSize(filters->filtersNames); i++) printf("%s", getArrayChar(toString, i));
    return toString;
}

ArrayChar *getFiltersNames() {
    return filters->filtersNames;
}

char *toLook(char * toLook) {
    for (int i = 0; i < getSize(*(filters->filtersNames)); i++) {
        //Se o nome for igual ao que está na lista, ele copia o path
        if (!strcmp(toLook,getArrayChar(filters->filtersNames, i))) return strdup(getArrayChar(filters->filtersPath, i));
    }
    return NULL;
}

int changeFilter(ArrayChar* listUsedFilters, int change){
    for (int i = 0; i < getSize(*(filters->filtersPath)); i++) {
        for (int j = 0; j < getSize(*(listUsedFilters)); j++) {
            if (!strcmp(getArrayChar((filters->filtersPath), i), getArrayChar(listUsedFilters, j))) {
                if (getArrayInt(filters->availableFilters, i) <= 0 && change < 0) return -1; 
                changeArrayInt(filters->availableFilters, i, change);
            }
        }

    }
    return 0;
}
struct Filters* duplicateFilters(){
    struct Filters* new = malloc(sizeof(struct Filters));
    new->filtersNames =  duplicateArrayChar(filters->filtersNames);
    new->filtersPath =  duplicateArrayChar(filters->filtersPath);


    new->availableFilters = duplicateArrayInt(filters->availableFilters);
    new->maxFilters =duplicateArrayInt(filters->maxFilters) ;
    return new;
}

void setFilters(struct Filters* toReplace){
    filters = toReplace;
}