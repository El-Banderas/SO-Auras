#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main() {
    int max_buffer = 100;
    char *path = "MYfifOServer";
    //mkfifo(path, 0666);
    char buffer[100];
    char buffer2[100];
    int fd;
    while (1) {
        fd = open(path, O_WRONLY);
        //Lê do terminal
        fgets(buffer, 100, stdin);
        write(fd, buffer, strlen(buffer) + 1);

        close(fd);
    }
    return 0;
}