#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

struct Filters {
   char  ** filters; //Name of filters
   int * availableFilters;
   int * totalFilters;
    int tam = 1;
   //Faltam reques;
} ;


struct Filters* initFilter() {
    struct Filters* new = malloc(sizeof(struct Filters));
    new->filters = malloc(sizeof(char * ) * 2);
    new->filter[0] = NULL;
    new->availableFilters = malloc(sizeof(int) * 2);
    new->availableFilters[0] = NULL;
    new->totalFilters = malloc(sizeof(int) * 2);
    new->totalFilters[0] = NULL;
    return new;
}

//void addFilter(char * filter)

int main(int argc, char *argv[]){
    FILE * fd;
    if (argc < 2) {
        perror("Insuficient argumetns");
        return -1;
    }
    fd = fopen(argv[1], "r");
    if (fd  == 0){
        perror("open");
        return -1;
    }
    else{
        struct Filters* all = initFilter();
        char * line = NULL; 
        size_t len = 0;
        size_t read;
        char buffer;

        printf("[DEBUG] Config file loaded\n");
        while ((read = getline(&line, &len, fd)) != -1) {
            printf("%s", line);
        }

    }
    fclose(fd);

}