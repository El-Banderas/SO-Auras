#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

/**
O cliente espera que o servidor crie o fifo privado (1 para 1), e depois começa a ler a partir dele.
*/
void ctrl_c_handler(int signum){
    char privateFifo[40];
    printf("Recebi o sinal\n");
    sprintf(privateFifo, "../tmp/%dFIFO$\n", getpid()); 
    char * path = strtok(privateFifo, "$");
    int fdPrivateFifo;
    printf("Path:'%s'\n", path);
    if ((fdPrivateFifo = open(path, O_RDONLY)) < 0) printf("private fifo not open\n");
    //fdPrivateFifo = open(path, O_RDONLY);
    char buffer[1024];
    int bytesRead = 0;
    while((bytesRead = read(fdPrivateFifo, buffer, 1024)) > 0) {
        write(1, buffer, bytesRead);
    }
    exit(0);
}

/**
Inicialmente, o cliente deve mandar o seu número de processo e o pedido.
Aqui não faz mais nada, para não confundir o seu pedido com o dos outros clientes.
*/
int main(int argc, char const *argv[]) {
    int fd = open("../tmp/centralFifo", O_WRONLY);
    int size_argv = 0;
    size_argv += 5;
    for (int i = 0; i < argc - 1; i++) size_argv += strlen(argv[i]);
    size_argv += (argc + 5);
    char currentPid[size_argv];
    sprintf(currentPid,"%d",getpid());
    
    for (int i = 0; i < argc - 1; i++) {
        strcat(currentPid, "$");
        strcat(currentPid, argv[i]);
    }
    
    strcat(currentPid,"$");
    strcat(currentPid,"End");

    //char currentPid[10];
    //sprintf(currentPid, "%d$coisas yey\n", getpid()); 
    
    write(fd, currentPid, size_argv);
    close(fd);
    if (signal(SIGINT, ctrl_c_handler) == SIG_ERR){
        perror("Erro com handler SIGINT");
        return -1;
    }
    while(1);
    return 0;
}
