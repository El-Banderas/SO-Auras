#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "request.h"

struct Request {
    char *inputName;
    char *outputName;

    char **filters;
    int sizeFilter;
};

Request createRequest(char *buffer) {
    return NULL;
}
