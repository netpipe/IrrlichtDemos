#ifndef XURFACED_CLIENT_H
#define XURFACED_CLIENT_H

struct xurfaced_client
{

    Window window;
    Picture picture;
    struct xurfaced_client *next;
    struct xurfaced_client *prev;

};

struct xurfaced_client_list
{

    struct xurfaced_client *head;
    struct xurfaced_client *current;

};

extern struct xurfaced_client *xurfaced_client_create(Window window);
extern void xurfaced_client_destroy(struct xurfaced_client *client);

extern struct xurfaced_client_list *xurfaced_client_list_create();
extern void xurfaced_client_list_destroy(struct xurfaced_client_list *list);
extern void xurfaced_client_list_add(struct xurfaced_client_list *list, struct xurfaced_client *client);
extern void xurfaced_client_list_remove(struct xurfaced_client_list *list, struct xurfaced_client *client);
extern struct xurfaced_client *xurfaced_client_list_find(struct xurfaced_client_list *list, Window window);

#endif

