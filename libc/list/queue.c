#include <stdlib.h>
#include <queue.h>

int queue_enqueue(queue_t *queue, const void *data)
{
    return list_ins_next(queue, list_tail(queue), data);
}

int queue_dequeue(queue_t *queue, void **data)
{
    return list_rem_next(queue, NULL, data);
}
