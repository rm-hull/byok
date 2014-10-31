#include <stdlib.h>
#include <string.h>

#include <stack_machine/history.h>

history_t *init_history(int size)
{
    assert(size > 0);

    history_t *hist = malloc(sizeof(history_t));
    if (hist != NULL)
    {
        hist->items = (char **)calloc(0, (size + 1) * sizeof(char *));
        hist->items[0] = "";    // always allow 1 item of blank history
        hist->count = 1;
        hist->size = size;
    }

    return hist;
}

void add_history(history_t *hist, char *text)
{
    assert(hist != NULL);

    // check to see if the text already exists in history
    // and if so quit early: no point in duplicating it
    int len = strlen(text);
    for (int i = 0, n = min(hist->count, hist->size); i < n; i++)
    {
        if (len == strlen(hist->items[i]) && memcmp(text, hist->items[i], len + 1) == 0)
            return;
    }

    char *copy = strdup(text);
    if (hist->count < hist->size)
    {
        hist->items[hist->count] = copy;
    }
    else
    {
        // Drop the item at the second slot (always retain the zeroth item - the blank)
        free(hist->items[1]);
        memmove(hist->items + 1, hist->items + 2, (hist->size - 2) * sizeof(char*));
        hist->items[hist->size - 1] = copy;
    }

    hist->count++;
}