#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include "request.h"
#include "filters.h"

#define bin "bin/aurrasd-filters/aurrasd-echo"
// <PID>.pipe




struct Request {
    pid_t pid;
    char *inputName;
    char *outputName;

    ArrayChar *filters; //Path
};

int countDollars(char *buffer) {
    //printf("To count dollars of '%s'", buffer);
    int res = 0;
    for (int i = 0; i < strlen(buffer); i++)
        if (buffer[i] == '$') res++;
    return res;
}

Request createRequest(char *buffer, int pidClient, char * path) {
    //sleep(10);
    int numArgs = countDollars(buffer);
    //Se tiver apenas 2 $ só tem $input$output$filtro$end\n
    if (numArgs < 4) {
        printf("Pedido '%s' inválido\n", buffer);
        return NULL;
    }
    buffer = &buffer[1];
    struct Request *new = malloc(sizeof(struct Request));
    new->pid = (pid_t) pidClient;

    //Send message

    const char s[2] = "$";

    strsep(&buffer, s);
    new->inputName = strsep(&buffer, s);
    //printf("2: %s\n", s1);

    new->outputName = strsep(&buffer, s);
    new->filters = initArrayChar(1);

    for (int i = 0; i < numArgs - 3; i++) {
        char * pathOfFilter = toLook(strsep(&buffer, s));
        if (pathOfFilter) insertArrayChar(new->filters, pathOfFilter);
        else {
            printf("One of the filter has an incorrect name\n");
        //      printf("request %s\n", buffer);
        }
    }
    //Verificar
    printf("Request information: \n");
    printf("InputName: %s\n", new->inputName);
    printf("OutputName: %s\n", new->outputName);
    for (int i = 0; i < getSize(*(new->filters)); i++)
        printf("FiltroPath %d: %s | ", i, getArrayChar(new->filters, i));
    printf("\n");
    //Acorda processo filho, mas já?
    //Avisar o filho que vai começar
    return new;
}


//Se não estiver a enviar, tirar o printf de comentário dentro do for
//Não sei porque funciona, mas funciona
void sendStatus(char *path, int pidClient) {
    //Avisar o filho que vai começar
    kill(pidClient, SIGUSR1);
    printf("Status is been sent.\n");
    ArrayChar *convertedToString = toString();
    //Abrir pipe privado
    int fd = open(path, O_WRONLY);
    //if ((fd = open(path, O_WRONLY)) < 0) perror("fifo load client not open\n");
    for (int i = 0; i < getSize(*getFiltersNames()); i++) {
        char *thisLine = getArrayChar(convertedToString, i);
        //printf("%s", thisLine);
        write(fd, thisLine, strlen(thisLine));
    }
    close(fd);
    sleep(2);
    kill(pidClient, SIGUSR2);
}
void sendMessage(char *path, int pidClient, char * message) {
    //Avisar o filho que vai começar
    kill(pidClient, SIGUSR1);
    //Abrir pipe privado
    int fd = open(path, O_WRONLY);
    //if ((fd = open(path, O_WRONLY)) < 0) perror("fifo load client not open\n");
    write(fd, message, strlen(message));
    close(fd);
}
int execFilter(char *filter) {
    char *args[10];
    char *tmp = strtok(filter, " ");

    int i;
    for (i = 0; tmp; i++) {
        args[i] = tmp;
        tmp = strtok(NULL, "");
    }
    args[i] = NULL;

    const char c[100] = "bin/aurrasd-filters/";
    char *path = strdup(c);
    strcat(path, filter);

    int ret = execlp(path, filter, NULL);

    return ret;
}

//O path é necessário para enviar a mensagem pelo fifo privado.
int runRequest(Request r, char * path) {
    //r -> filters = initArrayChar(1);
    //insertArrayChar(r -> filters, "aurrasd-gain-double");
    //insertArrayChar(r -> filters, "aurrasd-tempo-half");
    if ((getSize(*(r->filters))) < 1) {
        printf("Request not valid\n");
        return -1;
    }
    while (filtersMissing(r->filters)) pause();
    // O filho tem que ter o PID do pai, para quando acabar avisar este (PAI) que pode verificar se pode correr o request
    sendMessage(path, r->pid, "Request is starting\n");
    
    if (changeFilter(r->filters, -1) < 0) {
        printf("The filters are invalid 1 [REQUEST.C]\n");
        sendMessage(path, r->pid, "Invalid Request because of filters\n");
        return -1;
    }

    int input = open(r->inputName, O_RDONLY);
    if (input == -1) {
        perror("open");
        return -1;
    }
    printf("The input is: %d\n", input);

    int output = open(r->outputName, O_WRONLY | O_CREAT, 0666);
    if (output == -1) {
        perror("open");
        return -1;
    }


    int sizeFilter = getSize(*(r->filters));

    int pipes[sizeFilter - 1][2];

    if (sizeFilter == 1) {
        // Redirecionar o stdout e o stdin para o pipe
        dup2(input, 0);
        dup2(output, 1);
        close(input);
        close(output);
        switch (fork()) {
            case -1:
                perror("fork");
                return -1;

            case 0:
                execFilter(getArrayChar(r->filters, 0)); // "bin/aurrasd-filters/aurrasd-gain-half"
                _exit(0);

            default:
                close(input);
                wait(NULL);
                close(output);

        }
        return 0;
    } else {
        if (pipe(pipes[0]) != 0) {
            perror("pipe");
            return -1;
        }
        switch (fork()) {
            case -1:
                perror("fork");
                return -1;

            case 0:
                close(pipes[0][0]); // Fechar pipe de leitura

                // Redirecionar o stdout e o stdin para o pipe para o primeiro filtro
                dup2(pipes[0][1], 1);
                //printf("[DEBUG] SemDeadLockInput\n");
                close(pipes[0][1]);

                dup2(input, 0);
                close(input);
                
                execFilter(getArrayChar(r->filters, 0));
                _exit(0); // Caso dê erro no exec

            default:
                close(pipes[0][1]); // Fechar pipe de escrita
                wait(NULL);
                close(input);

        }
        for (int i = 1; i < sizeFilter - 1; i++) {
            if (pipe(pipes[i])) {
                perror("pipe");
                return -1;
            }
            switch (fork()) {
                case -1:
                    perror("fork");
                    return -1;

                case 0:
                    close(pipes[i][0]); // Fechar pipe de leitura

                    // Redirecionar o stdout e o stdin para o pipe para o primeiro filtro
                    dup2(pipes[i - 1][0], 0);
                    close(pipes[i - 1][0]);
                    dup2(pipes[i][1], 1);
                    close(pipes[i][1]);

                    execFilter(getArrayChar(r->filters, i));
                    _exit(0); // Caso dê erro no exec

                default:
                    close(pipes[i][1]); // Fechar pipe de escrita
                    close(pipes[i - 1][0]); // Fechar pipe de leitura

            }
        }

        int n = sizeFilter - 1;
            printf("Final part of excecution\n");
        switch (fork()) {
            case -1:
                perror("fork");
                return -1;

            case 0:
                //close(pipes[n - 1][0]); // Fechar pipe de leitura

                // Redirecionar o stdout e o stdin para o pipe para o primeiro filtro
                dup2(pipes[n - 1][0], 0);
                close(pipes[n - 1][0]);
                dup2(output, 1);
                close(output);

                execFilter(getArrayChar(r->filters, n));
                _exit(0); // Caso dê erro no exec

            default:
                close(output); // Fechar pipe de escrita
                close(pipes[n - 1][0]); // Fechar pipe de leitura
        }
            wait(NULL);
        
    }
    if (changeFilter(r->filters, 1) < 0) {
            printf("The filters are invalid 2 [REQUEST.C]\n");
            sendMessage(path, r->pid, "Invalid Request because of filters\n");
        return -1;
        }
    printf("Aumentou filtros\n");
    return 0;  //Fui eu que meti isto, o sujeito indefinido da frase é o Diogo
}

pid_t getPid(Request r) {
    return r->pid;
    }