#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "request.h"
#include "queue.h"

typedef struct RequestNode {
    Request request;
    struct RequestNode *next;
} *RequestNode;

struct Queue {
    RequestNode requests;
    size_t size;
};


Queue createQueue() {
    Queue queue = malloc(sizeof(struct Queue));
    queue -> requests = NULL;
    queue -> size = 0;

    return queue;
}

void enQueue(Queue q, Request r) {
    /*
    if (q -> requests == NULL) {
        q -> requests = malloc(sizeof(struct RequestNode));
        (q -> requests) -> request = r;
        (q -> requests) -> next = NULL;
    } else {
        q -> requests =
    }
     */

    q -> size++;
}