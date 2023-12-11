#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <client.h>

struct xurfaced_client *xurfaced_client_create(Window window)
{

    struct xurfaced_client *client = malloc(sizeof (struct xurfaced_client));
    client->window = window;
    client->picture = 0;
    client->next = 0;
    client->prev = 0;

    return client;

}

void xurfaced_client_destroy(struct xurfaced_client *client)
{

    free(client);

}

struct xurfaced_client_list *xurfaced_client_list_create()
{

    struct xurfaced_client_list *list = malloc(sizeof (struct xurfaced_client_list));
    list->head = 0;
    list->current = 0;

    return list;

}

void xurfaced_client_list_destroy(struct xurfaced_client_list *list)
{

    struct xurfaced_client *current = list->head;

    do
    {

        struct xurfaced_client *next = current->next;

        xurfaced_client_destroy(current);

        current = next;

    }
    while (current != list->head);

    list->current = 0;

}

void xurfaced_client_list_add(struct xurfaced_client_list *list, struct xurfaced_client *client)
{

    if (!list->head)
    {

        client->next = client;
        client->prev = client;

        list->head = client;

    }

    else
    {

        client->next = list->head;
        client->prev = list->head->prev;

        list->head->prev->next = client;
        list->head->prev = client;

    }

}

void xurfaced_client_list_remove(struct xurfaced_client_list *list, struct xurfaced_client *client)
{

    if (client == list->head)
        list->head = client->next;

    client->prev->next = client->next;
    client->next->prev = client->prev;

    if (client == client->next)
        list->head = 0;

}

struct xurfaced_client *xurfaced_client_list_find(struct xurfaced_client_list *list, Window window)
{

    struct xurfaced_client *current = list->head;

    if (!current)
        return 0;

    while (current->next != list->head)
    {

        if (current->window == window)
            return current;

        current = current->next;

    }

    return (current->window == window) ? current : 0;

}

