#include <stdlib.h>
#include <string.h>
#include <list.h>

void list_init(list_t *list, void (*destroy)(void *data))
{
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
}

void list_destroy(list_t *list)
{
    void *data;
    while (list_size(list) > 0)
    {
        if (list_rem_next(list, NULL, (void **)&data) == 0 && list->destroy != NULL)
            list->destroy(data);
    }
    memset(list, 0, sizeof(list_t));
}

int list_ins_next(list_t *list, list_elem_t *element, const void *data)
{
    list_elem_t *new_element;
    if ((new_element = (list_elem_t *)malloc(sizeof(list_elem_t))) == NULL)
        return -1;

    new_element->data = (void *)data;
    if (element == NULL)
    {
        if (list_size(list) == 0)
            list->tail = new_element;

        new_element->next = list->head;
        list->head = new_element;
    }
    else
    {
        if (element->next == NULL)
            list->tail = new_element;

        new_element->next = element->next;
        element->next = new_element;
    }

    list->size++;
    return 0;
}

int list_rem_next(list_t *list, list_elem_t *element, void **data)
{
    list_elem_t *old_element;

    if (list_size(list) == 0)
        return -1;

    if (element == NULL)
    {
        *data = list->head->data;
        old_element = list->head;
        list->head = list->head->next;

        if (list_size(list) == 0)
            list->tail = NULL;
    }
    else
    {
        if (element->next == NULL)
            return -1;

        *data = element->next->data;
        old_element = element->next;
        element->next = element->next->next;

        if (element->next == NULL)
            list->tail = element;
    }

    free(old_element);
    list->size--;
    return 0;
}
