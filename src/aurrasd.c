#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "basicOperations.h"

struct Filters {
    ArrayChar filtersNames; //Name of filters
    ArrayInt availableFilters;
    ArrayInt totalFilters;
    //Faltam reques;
};


struct Filters *initFilter() {
    struct Filters *new = malloc(sizeof(struct Filters));
    ArrayChar filtersNames;
    initArrayChar(&filtersNames, 4);
    new->filtersNames = filtersNames;
    ArrayInt availableFilters;
    ArrayInt totalFilters;
    initArrayInt(&availableFilters, 4);
    initArrayInt(&totalFilters, 4);

    new->availableFilters = availableFilters;
    new->totalFilters = totalFilters;
    return new;
}

//void addFilter(char * filter)


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
        int numChar = 10;
        char buffer[numChar];
        char *final;
        while (read(fd, &buffer, numChar) > 0)
            final = append_strings(final, buffer);

        printf("[DEBUG] Config file loaded\n");
        printf("%s", final);

        close(fd);
    }

}