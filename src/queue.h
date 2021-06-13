#ifndef SO___AURAS_QUEUE_H
#define SO___AURAS_QUEUE_H

#include "request.h"

typedef struct Queue *Queue;

Queue createQueue();

void enQueue(Queue q, Request r);

#endif //SO___AURAS_QUEUE_H
