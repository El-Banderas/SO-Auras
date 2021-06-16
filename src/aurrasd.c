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


extern struct Filters *filters;




struct Filters * readConfig(char * path){
    int fd = open(path, O_RDONLY);
    if (fd == 0) {
        perror("open");
        return NULL;
    } else {
        struct Filters *all = initFilterStructur();
        int numChar = 526;
        char buffer[numChar];
        while (readln(fd, buffer, numChar) > 2) {
    printf("Aqui!\n");
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
                        // tmp/<PID>.pipe
    
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
          Request r = createRequest(buffer);
          runRequest(r);
        }
    
        if (bytesRead == 0){
            printf("[DEBUG]: End of one client\n");
        }
    
    close(fifofd);
    }
  
    return 0;
}