#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include "request.h"
#include "filters.h"

#define bin "aurrasd-filters"
// <PID>.pipe




struct Request {
    pid_t pid;
    char *inputName;
    char *outputName;

    ArrayChar *filters;
};

int countDollars(char *buffer) {
    //printf("To count dollars of '%s'", buffer);
    int res = 0;
    for (int i = 0; i < strlen(buffer); i++)
        if (buffer[i] == '$') res++;
    return res;
}

Request createRequest(char *buffer, int pidClient) {
    //sleep(10);
    int numArgs = countDollars(buffer);
    //Se tiver apenas 2 $ só tem $input$output$filtro$end\n
    if (numArgs < 5) {
        printf("Pedido '%s' inválido\n", buffer);
        return NULL;
    }
    buffer = &buffer[1];
    struct Request *new = malloc(sizeof(struct Request));
    new->pid = (pid_t) pidClient;
    const char s[2] = "$";

    char *lixo = strsep(&buffer, s);
    new->inputName = strsep(&buffer, s);
    //printf("2: %s\n", s1);

    new->outputName = strsep(&buffer, s);
    new->filters = initArrayChar(1);

    for (int i = 0; i < numArgs - 4; i++) {
        insertArrayChar(new->filters, strsep(&buffer, s));
        //      printf("request %s\n", buffer);
    }
    //Verificar
    printf("O request criado é: \n");
    printf("InputName: %s\n", new->inputName);
    printf("OutputName: %s\n", new->outputName);
    for (int i = 0; i < getSize(*(new->filters)); i++)
        printf("Filtro %d: %s | ", i, getArrayChar(new->filters, i));
    //Acorda processo filho, mas já?
    //Avisar o filho que vai começar
    return NULL;
}

//Se não estiver a enviar, tirar o printf de comentário dentro do for
//Não sei porque funciona, mas funciona
void sendStatus(char *path, int pidClient) {
    //Avisar o filho que vai começar
    kill(pidClient, SIGUSR1);
    //printf("Status is here\n");
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
}

int execCommand(char *command) {
    char *args[10];
    char *tmp = strtok(command, " ");

    int i;
    for (i = 0; tmp; i++) {
        args[i] = tmp;
        tmp = strtok(NULL, "");
    }
    args[i] = NULL;

    int ret = execvp(args[0], args);

    return ret;
}


int runRequest(Request r) {
    printf("Passou\n");
    r = malloc(sizeof(struct Request));
    r->inputName = strdup("samples/sample-1-so.m4a");
    r->filters = initArrayChar(3);
    insertArrayChar(r->filters, "alto");
    insertArrayChar(r->filters, "eco");
    insertArrayChar(r->filters, "rapido");
    r->outputName = "output.m4a";

    while (filtersMissing(r->filters)) pause();
    // O filho tem que ter o PID do pai, para quando acabar avisar este (PAI) que pode verificar se pode correr o request

    int input = open(r->inputName, O_RDONLY);
    if (input == -1) {
        perror("open");
        return -1;
    }

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
                execCommand(getArrayChar(r->filters, 0));
                _exit(0);


        }
        return 0;
    } else {
        if (pipe(pipes[0])) {
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
                dup2(input, 0);
                close(input);
                dup2(pipes[0][1], 1);
                close(pipes[0][1]);
                printf("SemDeadLock\n");
                execCommand(getArrayChar(r->filters, 0));
                _exit(0); // Caso dê erro no exec

            default:
                close(pipes[0][1]); // Fechar pipe de escrita

        }
        for (int i = 1; i < sizeFilter - 1; i++) {
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

                    execCommand(getArrayChar(r->filters, i));
                    _exit(0); // Caso dê erro no exec

                default:
                    close(pipes[i][1]); // Fechar pipe de escrita
                    close(pipes[i - 1][0]); // Fechar pipe de leitura

            }
        }

        int n = sizeFilter - 1;
        switch (fork()) {
            case -1:
                perror("fork");
                return -1;

            case 0:
                close(pipes[n - 1][0]); // Fechar pipe de leitura

                // Redirecionar o stdout e o stdin para o pipe para o primeiro filtro
                dup2(pipes[n - 1][0], 0);
                close(pipes[n - 1][0]);
                dup2(output, 1);
                close(output);

                execCommand(getArrayChar(r->filters, n));
                _exit(0); // Caso dê erro no exec

            default:
                close(output); // Fechar pipe de escrita
                close(pipes[n - 1][0]); // Fechar pipe de leitura
        }

        return 0;
    }
    return -1;  //Fui eu que meti isto, o sujeito indefinido da frase é o Diogo
}
