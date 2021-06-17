#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include "request.h"
#include "filters.h"

int running = 1; // Variavel de status, para ver se o programa continua a receber pedidos

int readConfig(char *path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 0;
    } else {
        initFilterStructur();
        int numChar = 526;
        char buffer[numChar];
        while (readln(fd, buffer, numChar) > 2) {
            /*if (buffer != NULL && strlen(buffer) > 2)*/ addFilter(buffer);
            for (int i = 0; buffer[i] != '\0' && i < numChar; i++) buffer[i] = '\0';
        }
        printf("[DEBUG] Config file loaded\n");
        toString();

        close(fd);
    }

    return 1;
}

void ctrl_c_handeler(int signum) {
    printf("\n[DEBUG] - Ending the program by ctrl-C\n");
    running = 0;
}


/**
Quando chega um pedido, ele cria o fifo pessoal.
Depois está a escrever mensagens para fazer debugg.
*/
void loadClient(char *buffer) {
    char privateFifo[40];
    const char s[2] = "$";
    char *ptr;

    int pidClient = (int) strtol(buffer, &ptr, 10);

    sprintf(privateFifo, "tmp/%d.pipe$", pidClient);
    char *path = strtok(privateFifo, "$");
    mkfifo(path, 0644);

    //int fd;
    printf("\nPath:'%s'\n", path);
    //fd = open(path, O_WRONLY);
    //if ((fd = open(path, O_WRONLY)) < 0) perror("fifo load client not open\n");
    //printf("Aqui\n");

    //Caso queira ver se o fifo funciona
    //if( write(fd, "Olá do servidor\n", 18) < 0) perror ("Write to pipe ;)\n");
//    close(fd);
    char *full = strdup(ptr);
    char *request = strtok(ptr, s);
    //Verifica se o pedido é um status
    char *status = "status";
    //printf("Current request is:'%s' and '%s' \n", request, status);
    int isStatus = 0;
    //Verifica se o pedido é status
    if (strlen(request) >= 5) for (; status[isStatus] != '\0' && status[isStatus] == request[isStatus]; isStatus++);
    //printf("Valores : %ld %ld %d \n", strlen(request), strlen(status), isStatus);
    if (isStatus == 6) sendStatus(path, pidClient);
    else {
        if (!fork()) {
            Request r = createRequest(full, pidClient, path);
            sendMessage(path, getPid(r), "Pending...\n");
            if (runRequest(r, path) == -1) {
                char clientFifo[40];
                sprintf(clientFifo, "tmp/%d.pipe", pidClient);
                kill(pidClient, SIGUSR1);
                int fd = open(clientFifo, O_WRONLY);
                char *err = "Erro!\n";
                write(fd, err, strlen(err));
                close(fd);
                kill(pidClient, SIGUSR2);
            } else {
                kill(pidClient, SIGUSR1);
                int fd = open(path, O_WRONLY);
                char *msg = "Done";
                write(fd, msg, strlen(msg));
                close(fd);
                kill(pidClient, SIGUSR2);
            }
            _exit(0);
        }
        printf("\nPassouRun\n");
    }
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        perror("Insuficient argumets\n");
        return -1;
    }
    //Guarda o ficheiro .config
    int config_ret = readConfig(argv[1]);
    if (!config_ret) {
        perror("Config not loaded\n");
        return -1;
    }
    if (signal(SIGINT, ctrl_c_handeler) == SIG_ERR) {
        perror("Erro com handler SIGINT");
        return -1;
    }
    //Cria o fifo central, onde os clientes mandam pedidos
    char *pathCentralFIfo = "tmp/centralFifo";
    mkfifo(pathCentralFIfo, 0644);
    int fifofd;
    int bytesRead = 0;
    char buffer[1024];
    while (running) {
        //Lê pedidos do fico central
        if ((fifofd = open(pathCentralFIfo, O_RDONLY)) < 0) {
            perror("fifo not open\n");
            return -1;
        } else printf("[DEBUG] : fifo geral open\n");
        //Carrega cada pedido, não faz já o request porque precisa do fifo privado
        buffer[0] = '\0';
        while ((bytesRead = readln(fifofd, buffer, 1024)) > 2) {
            if (buffer[0] != '\0' && strlen(buffer) > 2) {
                //printf("Servidor recebeu: %s", buffer);
                loadClient(buffer);
            }
        }
        //if (bytesRead == 0){
        printf("[DEBUG]: End of one client\n");
        close(fifofd);
    }

    pid_t pid;
    while ((pid = wait(NULL)) > 0) printf("[DEBUG] Filho com o pid %d terminou!", pid);

    unlink(pathCentralFIfo);

    return 0;
}