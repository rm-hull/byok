#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <list.h>

typedef list_t queue_t;

#define queue_init list_init
#define queue_destroy list_destroy
#define queue_peek(queue) ((queue)->head == NULL ? NULL : (queue)->head->data)
#define queue_size list_size

extern int queue_enqueue(queue_t *queue, const void *data);
extern int queue_dequeue(queue_t *queue, void **data);

#endif
