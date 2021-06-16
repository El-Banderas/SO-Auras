#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include "basicOperations.h"
#include "request.h"

#define bin "bin/aurrasd-filters"
// <PID>.pipe


struct Filters {
    ArrayChar *filtersNames; // Há diferença entre o path e o outro
    ArrayChar *filtersPath; // Name of filters
    ArrayInt *availableFilters;
    ArrayInt *maxFilters;
    //Faltam reques;
};

struct Filters *filters = NULL;/*{
        .filtersNames = NULL,
        .filtersPath = NULL,
        .availableFilters = NULL,
        .maxFilters = 0
};*/

struct Filters* initFilterStructur() {
    struct Filters* filters = malloc(sizeof(struct Filters));
    ArrayChar* filtersNames = NULL;
    ArrayChar* filtersPath= NULL;
    initArrayChar(filtersNames, 2);
    initArrayChar(filtersPath, 2);
    filters->filtersNames = filtersNames;
    filters->filtersPath = filtersPath;
    ArrayInt* availableFilters= NULL;
    ArrayInt* maxFilters= NULL;
    initArrayInt(availableFilters, 2);
    initArrayInt(maxFilters, 2);

    filters->availableFilters = availableFilters;
    filters->maxFilters = maxFilters;
    return filters;
}

void addFilter(char *filter, struct Filters *current) {
    char name[100];
    char path[100];
    int available;
    //printf("AddFilter: %s \n", filter, strlen(filter));
    sscanf(filter, "%s %s %d", name, path, &available);
    // printf("Name %s\nPath %s\n Number %d\n ", name, path, available);
    insertArrayChar(current->filtersNames, name);
    insertArrayChar(current->filtersPath, path);
    insertArrayInt(current->availableFilters, available);
    insertArrayInt(current->maxFilters, available);
//    printf("%d %s\n", 0, getArrayChar(&(current->filtersNames) , 0));
}

// @Override
void toString(struct Filters *x) {
    for (int i = 0; i < getSize(*(x->filtersNames)); i++)
        printf("To String %d %s\n", i, getArrayChar(x->filtersNames, i));
}

struct Request {
    pid_t pid;
    char *inputName;
    char *outputName;

    char **filters;
    int sizeFilter;
};

Request createRequest(char *buffer) {
    printf("%s\n", buffer);
    return NULL;
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

char filtersMissing(Request r) {
    return 0;
}

int runRequest(Request r) {
    while (filtersMissing(r)) pause();
    // O filho tem que ter o PID do pai, para quando acabar avisar este (PAI) que pode verificar se pode correr o request

    int output = open(r->outputName, O_WRONLY);
    int input = open(r->inputName, O_RDONLY);

    int pipes[r->sizeFilter - 1][2];

    if (r->sizeFilter == 1) {
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
                execCommand(r->filters[0]);
                _exit(0);


        }
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

                execCommand((r->filters)[0]);
                _exit(0); // Caso dê erro no exec

            default:
                close(pipes[0][1]); // Fechar pipe de escrita

        }
        for (int i = 1; i < r->sizeFilter - 1; i++) {
            // cenas
        }

        int n = r->sizeFilter - 1;
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

                execCommand((r->filters)[n]);
                _exit(0); // Caso dê erro no exec

            default:
                close(pipes[n - 1][1]); // Fechar pipe de escrita

        }

        return 0;
    }
    return -1;  //Fui eu que meti isto, o sujeito indefinido da frase é o Diogo
}
