#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "basicOperations.h"
#include "request.h"

struct Filters {
    ArrayChar filtersNames; //Há diferença entre o path e o outro
    ArrayChar filtersPath; //Name of filters
    ArrayInt availableFilters;
    ArrayInt maxFilters;
    //Faltam reques;
};


struct Filters *initFilterStructur() {
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

struct Filters * readConfig(char * path){
    int fd = open(path, O_RDONLY);
    if (fd == 0) {
        perror("open");
        return NULL;
    } else {
        struct Filters *all = initFilterStructur();
        int numChar = 526;
        char buffer[numChar];
        while (readln(fd, buffer, numChar) > 2  )
        {
            /*if (buffer != NULL && strlen(buffer) > 2)*/ addFilter(buffer, all);
            for (int i = 0; buffer[i] != '\0' && i < numChar; i++) buffer[i] = '\0';
        }
        printf("[DEBUG] Config file loaded\n");
        toString(all);

        close(fd);
        return all;
    }
} 
/**
Quando chega um pedido, ele cria o fifo pessoal.
Depois está a escrever mensagens para fazer debugg.
*/
void loadClient(char * buffer){
    char privateFifo[40];
    const char s[2] = "$";
    char *ptr;
    int pidChild = (int) strtol(strtok(buffer, s), &ptr, 10);
    sprintf(privateFifo, "tmp/%dFIFO$", pidChild); 
    char * path = strtok(privateFifo, "$");
    mkfifo(path, 0644);
    int fd;
    printf("Path:'%s'\n", path);
    kill(pidChild, SIGINT);
    fd = open(path, O_WRONLY);
    //if ((fd = open(path, O_WRONLY)) < 0) perror("fifo load client not open\n");
    printf("Aqui\n");
    if( write(fd, "Olá do servidor\n", 18) < 0) perror ("Write to pipe ;)\n");
    createRequest(ptr);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Insuficient argumets\n");
        return -1;
    }
    //Guarda o ficheiro .config
    struct Filters * config = readConfig(argv[1]);
    if (!config) {
        perror("Config not loaded\n");
        return -1;
    }
    //Cria o fifo central, onde os clientes mandam pedidos
    char *pathCentralFIfo = "tmp/centralFifo";
    mkfifo(pathCentralFIfo, 0644);
    int fifofd ; 
    int bytesRead = 0;
    char buffer[1024];
    while(1){
        //Lê pedidos do fico central
        if((fifofd = open(pathCentralFIfo, O_RDONLY)) < 0){
            perror("fifo not open\n");
            return -1;
        } 
        else printf("[DEBUG] : fifo geral open\n");
        //Carrega cada pedido, não faz já o request porque precisa do fifo privado
        while ((bytesRead = readln(fifofd, buffer, 1024)) > 0){
          loadClient(buffer);
        }
        if (bytesRead == 0){
            printf("[DEBUG]: End of one client\n");
        }
    close(fifofd); 
    }
    return 0;
}