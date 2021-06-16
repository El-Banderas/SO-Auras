#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include "basicOperations.h"
#include "request.h"
#include "filters.h"



struct Filters {
    ArrayChar *filtersNames; // Há diferença entre o path e o outro
    ArrayChar *filtersPath; // Name of filters
    ArrayInt *availableFilters;
    ArrayInt *maxFilters;
    //Faltam reques;
};

//struct Filters *filters = NULL;
/*{
        .filtersNames = NULL,
        .filtersPath = NULL,
        .availableFilters = NULL,
        .maxFilters = 0
};*/

struct Filters* initFilterStructur() {
    struct Filters* filters = malloc(sizeof(struct Filters));
    filters->filtersNames = initArrayChar(2);
    filters->filtersPath = initArrayChar(2);
    

    filters->availableFilters = initArrayInt(2);
    filters->maxFilters = initArrayInt(2);
    return filters;
}

void addFilter(char *filter, struct Filters *current) {
    char name[100];
    char path[100];
    int available;
    //printf("AddFilter: %s \n", filter, strlen(filter));
    sscanf(filter, "%s %s %d", name, path, &available);
    // printf("Name %s\nPath %s\n Number %d\n ", name, path, available);
    insertArrayChar(current->filtersNames, name);
    insertArrayChar(current->filtersPath, path);
    insertArrayInt(current->availableFilters, available);
    insertArrayInt(current->maxFilters, available);
//    printf("%d %s\n", 0, getArrayChar(&(current->filtersNames) , 0));
}

// @Override
ArrayChar * toString(struct Filters *x) {
    ArrayChar * toString = initArrayChar(2);
    insertArrayChar(toString, "Aqui inserem-se os requestes\n");

    for (int i = 0; i < getSize(*(x->filtersNames)); i++){
        char buffer[300];
        int total = getArrayInt((x->maxFilters), i);
        int running = total - getArrayInt((x->availableFilters), i);
        sprintf(buffer, "Filter %s: %d / %d (runing/max)\n", getArrayChar((x->filtersNames), i), running, total);
        insertArrayChar(toString, buffer);
        /*
        strcat(buffer, getArrayChar(x->filtersNames));
        strcat(buffer, ": ");
        */
 //       printf("To String %d %s\n", i, getArrayChar(&(x->filtersNames), i));
    }
    //for (int i = 0; i < getSize(x->filtersNames); i++) printf("%s", getArrayChar(toString, i));
    return toString;
}

ArrayChar* getFiltersNames(struct Filters *x){
    return x->filtersNames;
}