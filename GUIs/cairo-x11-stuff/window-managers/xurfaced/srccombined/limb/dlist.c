#include <stdlib.h>
#include <stdio.h>
#include <limb/dlist.h>

struct limb_ditem *limb_ditem_create(void *data)
{

    struct limb_ditem *item = (struct limb_ditem *)malloc(sizeof (struct limb_ditem));
    item->data = data;
    item->next = 0;

    return item;

}

void limb_ditem_destroy(struct limb_ditem *item, void (*pre_callback)(void *data))
{

    if (pre_callback)
        pre_callback(item->data);

    free(item);

}

void limb_dlist_add(struct limb_dlist *list, struct limb_ditem *item)
{

    struct limb_ditem *current;

    for (current = list->head; current && current->next; current = current->next);

    if (current)
        current->next = item;
    else
        list->head = item;

}

struct limb_dlist *limb_dlist_concat(struct limb_dlist *list1, struct limb_dlist *list2)
{

    struct limb_ditem *current;

    for (current = list1->head; current && current->next; current = current->next);

    if (current)
        current->next = list2->head;
    else
        list1->head = list2->head;

    list2->head = 0;

    return list1;

}

unsigned int limb_dlist_count(struct limb_dlist *list)
{

    struct limb_ditem *current;
    unsigned int i = 0;

    for (current = list->head; current; current = current->next)
        i++;

    return i;

}

struct limb_dlist *limb_dlist_create()
{

    struct limb_dlist *list = (struct limb_dlist *)malloc(sizeof (struct limb_dlist));
    list->head = 0;

    return list;

}

void limb_dlist_destroy(struct limb_dlist *list)
{

    free(list);

}

void limb_dlist_destroy_children(struct limb_dlist *list, void (*pre_callback)(void *data))
{

    struct limb_ditem *current;

    for (current = list->head; current; current = current->next)
        limb_ditem_destroy(current, pre_callback);

}

void limb_dlist_foreach(struct limb_dlist *list, void (*callback)(void *data))
{

    struct limb_ditem *current;

    for (current = list->head; current; current = current->next)
        callback(current->data);

}

void limb_dlist_remove(struct limb_dlist *list, struct limb_ditem *item)
{

    struct limb_ditem *current;

    for (current = list->head; current && current->next == item; current = current->next);

    if (!current)
        return;

    if (current == list->head)
        list->head = current->next;
    else
        current->next = current->next->next;

}

struct limb_diterator *limb_diterator_create(struct limb_dlist *list)
{

    struct limb_diterator *iterator = (struct limb_diterator *)malloc(sizeof (struct limb_diterator));
    iterator->list = list;
    iterator->current = 0;

    return iterator;

}

void limb_diterator_destroy(struct limb_diterator *iterator)
{

    free(iterator);

}

struct limb_ditem *limb_diterator_next(struct limb_diterator *iterator)
{

    return iterator->current = (iterator->current) ? iterator->current->next : iterator->list->head;

}

void limb_diterator_reset(struct limb_diterator *iterator)
{

    iterator->current = 0;

}

