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
} ;


struct Filters* initFilter() {
    struct Filters* new = malloc(sizeof(struct Filters));
    ArrayChar filtersNames;  
    initArrayChar(&filtersNames, 4);
    new->filtersNames = filtersNames  ;
    ArrayInt availableFilters;
    ArrayInt totalFilters;
    initArrayInt(&availableFilters, 4);
    initArrayInt(&totalFilters, 4);

    new->availableFilters = availableFilters; 
    new->totalFilters = totalFilters;
    return new;
}

//void addFilter(char * filter)


int main(int argc, char *argv[]){
    int fd;
    if (argc < 2) {
        perror("Insuficient argumetns");
        return -1;
    }
    fd = open(argv[1], O_RDONLY);
    if (fd  == 0){
        perror("open");
        return -1;
    }
    else{
        struct Filters* all = initFilter();
        int numChar = 10;
        char buffer[numChar+1];
        char * final= (char *) malloc(sizeof(char) * numChar);
        
        int temp= 0;
        char * final;
        final = readl3(fd);
        //while( ( temp = read(fd, final+temp, numChar)) >= 0 ){
        //    numChar += 10;
        //    final =  realloc(final, numChar * (sizeof(char *)));
        //    temp+=10;
        //}
        //temp =read(fd, final, numChar);
//        final[numChar+1] = '\0';
 //       if (temp == numChar) 
  //      {

        printf("[DEBUG] Config file loaded\n");
            printf("%s", final);

    close(fd);
    }

}