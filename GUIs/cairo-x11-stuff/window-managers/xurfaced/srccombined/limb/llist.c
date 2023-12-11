#include <stdlib.h>
#include <stdio.h>
#include <limb/llist.h>

struct limb_litem *limb_litem_create(void *data)
{

    struct limb_litem *item = (struct limb_litem *)malloc(sizeof (struct limb_litem));
    item->data = data;
    item->next = 0;

    return item;

}

void limb_litem_destroy(struct limb_litem *item, void (*pre_callback)(void *data))
{

    if (pre_callback)
        pre_callback(item->data);

    free(item);

}

void limb_llist_add(struct limb_llist *list, struct limb_litem *item)
{

    struct limb_litem *current;

    for (current = list->head; current && current->next; current = current->next);

    if (current)
        current->next = item;
    else
        list->head = item;

}

struct limb_llist *limb_llist_concat(struct limb_llist *list1, struct limb_llist *list2)
{

    struct limb_litem *current;

    for (current = list1->head; current && current->next; current = current->next);

    if (current)
        current->next = list2->head;
    else
        list1->head = list2->head;

    list2->head = 0;

    return list1;

}

unsigned int limb_llist_count(struct limb_llist *list)
{

    struct limb_litem *current;
    unsigned int i = 0;

    for (current = list->head; current; current = current->next)
        i++;

    return i;

}

struct limb_llist *limb_llist_create()
{

    struct limb_llist *list = (struct limb_llist *)malloc(sizeof (struct limb_llist));
    list->head = 0;

    return list;

}

void limb_llist_destroy(struct limb_llist *list)
{

    free(list);

}

void limb_llist_destroy_children(struct limb_llist *list, void (*pre_callback)(void *data))
{

    struct limb_litem *current;

    for (current = list->head; current; current = current->next)
        limb_litem_destroy(current, pre_callback);

}

void limb_llist_foreach(struct limb_llist *list, void (*callback)(void *data))
{

    struct limb_litem *current;

    for (current = list->head; current; current = current->next)
        callback(current->data);

}

void limb_llist_remove(struct limb_llist *list, struct limb_litem *item)
{

    struct limb_litem *current;

    for (current = list->head; current && current->next == item; current = current->next);

    if (!current)
        return;

    if (current == list->head)
        list->head = current->next;
    else
        current->next = current->next->next;

}

struct limb_literator *limb_literator_create(struct limb_llist *list)
{

    struct limb_literator *iterator = (struct limb_literator *)malloc(sizeof (struct limb_literator));
    iterator->list = list;
    iterator->current = 0;

    return iterator;

}

void limb_literator_destroy(struct limb_literator *iterator)
{

    free(iterator);

}

struct limb_litem *limb_literator_next(struct limb_literator *iterator)
{

    return iterator->current = (iterator->current) ? iterator->current->next : iterator->list->head;

}

void limb_literator_reset(struct limb_literator *iterator)
{

    iterator->current = 0;

}

