#ifndef _HISTORY_H
#define _HISTORY_H

typedef struct
{
    char **items;
    int count;
    int size;
} history_t;

extern history_t *init_history(int size);
extern void add_history(history_t *hist, char *text);

#endif