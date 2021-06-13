#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
//int mkfifo(const char *pathname, mode_t mode);




int main(){
    int max_buffer = 100;
    char * path = "MYfifOServer";
    //mkfifo(path, 0666);
    char buffer[100];
    char buffer2[100];
    int fd;
    //meter write only
     FILE *fdlog=  fopen("log.val", "w":q
     );  
    while (1){
     fd =  open(path, O_RDONLY);  
     //Lê do terminal
     read(fd, buffer, 100);
     printf("O write.c escreveu: %s\n", buffer);
     fprintf(fdlog, "%s\n", buffer);
     close(fd);
    }
    return 0;
}




//DO PROFESSOR
/*
int main(int argc, char *arvg()){
    char buf[1024];
    int bytes_read ;
    
    if(mkfifo("fifo", 0666) == -1){
        perror("Mikefifo");
        return -1;
    }
    else printf("Opened\n");
    int fd;
    if ((fd = open("fifo", O_RDONLY) == -1)){
        perror("open");
        return -1;
    }
    else printf("After open\n");
    while(bytes_read = read(0, &buf, 1024)){
        write(1, &buf, bytes_read);
        printf("Read something\n");
    }
    if (bytes_read == 0){
        printf("EOF\n");
    }
    else perror("Read");
    close(fd);
    unlink("fifo");
    return 0;



}
*/