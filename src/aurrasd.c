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
            /*if (buffer != NULL && strlen(buffer) > 2)*/ addFilter(buffer, all);
            for (int i = 0; buffer[i] != '\0' && i < numChar; i++) buffer[i] = '\0';
        }
        printf("[DEBUG] Config file loaded\n");
        toString(all);

        close(fd);
        return all;
    }
}

void ctrl_c_handeler(int signum){
    printf("[DEBUG] - End of program by ctrl-C\n");
    exit(0);
}


/**
Quando chega um pedido, ele cria o fifo pessoal.
Depois está a escrever mensagens para fazer debugg.
*/
void loadClient(char * buffer, struct Filters *all){
    char privateFifo[40];
    const char s[2] = "$";
    char *ptr;

    int pidChild = (int) strtol(buffer, &ptr, 10);
    
    sprintf(privateFifo, "tmp/%d.pipe$", pidChild);
    char * path = strtok(privateFifo, "$");
    mkfifo(path, 0644);

    //int fd;
    printf("\nPath:'%s'\n", path);
    //fd = open(path, O_WRONLY);
    //if ((fd = open(path, O_WRONLY)) < 0) perror("fifo load client not open\n");
    //printf("Aqui\n");

    //Caso queira ver se o fifo funciona
    //if( write(fd, "Olá do servidor\n", 18) < 0) perror ("Write to pipe ;)\n");
//    close(fd);
    char * full = strdup(ptr);
    char * request =strtok(ptr, s); 
    //Verifica se o pedido é um status
    char *status = "status";
    //printf("Current request is:'%s' and '%s' \n", request, status);
    int isStatus = 0;
    //Verifica se o pedido é status
    if (strlen(request) >= 5) for (; status[isStatus] != '\0' &&  status[isStatus] == request[isStatus]; isStatus++);
    //printf("Valores : %ld %ld %d \n", strlen(request), strlen(status), isStatus);
    if (isStatus == 6) sendStatus(all, path, pidChild);
    else createRequest(full, pidChild);
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
    if (signal(SIGINT, ctrl_c_handeler) == SIG_ERR){
        perror("Erro com handler SIGINT");
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
        buffer[0] = '\0';
        while ((bytesRead = readln(fifofd, buffer, 1024)) > 2){
            if (buffer[0] != '\0' && strlen(buffer) > 2) {
                printf("Servidor recebeu: %s", buffer);
                loadClient(buffer, config);}
        }
        //if (bytesRead == 0){
        printf("[DEBUG]: End of one client\n");
    close(fifofd);
    }
    return 0;
}