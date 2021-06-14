#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "basicOperations.h"

struct Filters {
    ArrayChar filtersNames; //Há diferença entre o path e o outro
    ArrayChar filtersPath; //Name of filters
    ArrayInt availableFilters;
    ArrayInt maxFilters;
    //Faltam reques;
};


struct Filters *initFilter() {
    struct Filters *new = malloc(sizeof(struct Filters));
    ArrayChar filtersNames;
    ArrayChar filtersPath;
    initArrayChar(&filtersNames, 2);
    initArrayChar(&filtersPath, 2);
    new->filtersNames = filtersNames;
    new->filtersPath = filtersPath;
    ArrayInt availableFilters;
    ArrayInt maxFilters;
    initArrayInt(&availableFilters, 2);
    initArrayInt(&maxFilters, 2);

    new->availableFilters = availableFilters;
    new->maxFilters = maxFilters;
    return new;
}

void addFilter(char * filter, struct Filters *current){
    char name[100];
    char path[100];
    int available;
    //printf("AddFilter: %s \n", filter, strlen(filter));
    sscanf(filter, "%s %s %d", name, path, &available );
   // printf("Name %s\nPath %s\n Number %d\n ", name, path, available);
    insertArrayChar(&(current->filtersNames), name);
    insertArrayChar(&(current->filtersPath), path);
    insertArrayInt(&(current->availableFilters), available);
    insertArrayInt(&(current->maxFilters), available);
//    printf("%d %s\n", 0, getArrayChar(&(current->filtersNames) , 0));
}

void toString(struct Filters * x){
    for (int i = 0; i < getSize(x->filtersNames); i++) printf("To String %d %s\n", i, getArrayChar(&(x->filtersNames), i));
}

int main(int argc, char *argv[]) {
    int fd;
    if (argc < 2) {
        perror("Insuficient argumetns");
        return -1;
    }
    fd = open(argv[1], O_RDONLY);
    if (fd == 0) {
        perror("open");
        return -1;
    } else {
        struct Filters *all = initFilter();
        int numChar = 526;
        char buffer[numChar];
        while (readln(fd, buffer, numChar) > 2  )
        {
            /*if (buffer != NULL && strlen(buffer) > 2)*/ addFilter(buffer, all);
            for (int i = 0; buffer[i] != '\0' && i < numChar; i++) buffer[i] = '\0';
        }
        printf("[DEBUG] Config file loaded\n");
        printf("\n\nAfter writing\n\n");
        toString(all);

        close(fd);
    }

}