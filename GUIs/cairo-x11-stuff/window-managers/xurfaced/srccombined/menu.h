#ifndef XURFACED_MENU_H
#define XURFACED_MENU_H

struct xurfaced;
struct animation_properties;

struct xurfaced_menu_option
{

    unsigned int type;
    char *name;
    char *description;
    char *command;
    struct animation_properties animationProperties;
    struct xurfaced_menu_option *next;
    struct xurfaced_menu_option *prev;

};

struct xurfaced_menu_option_list
{

    struct xurfaced_menu_option *head;
    struct xurfaced_menu_option *current;

};

struct xurfaced_menu
{

    struct xurfaced_menu_option_list *opts;
    struct animation_properties animationProperties;

};

extern void xurfaced_execute(char *command, int pipe[]);

extern struct xurfaced_menu_option *xurfaced_menu_option_create();
extern void xurfaced_menu_option_destroy(struct xurfaced_menu_option *option);

extern struct xurfaced_menu_option_list *xurfaced_menu_option_list_create();
extern void xurfaced_menu_option_list_destroy(struct xurfaced_menu_option_list *list);
extern void xurfaced_menu_option_list_add(struct xurfaced_menu_option_list *list, struct xurfaced_menu_option *option);
extern void xurfaced_menu_option_list_remove(struct xurfaced_menu_option_list *list, struct xurfaced_menu_option *option);

extern struct xurfaced_menu *xurfaced_menu_create();
extern void xurfaced_menu_destroy(struct xurfaced_menu *menu);
extern void xurfaced_menu_activate(struct xurfaced_menu *menu);
extern void xurfaced_menu_next(struct xurfaced_menu *menu, unsigned int num);
extern void xurfaced_menu_previous(struct xurfaced_menu *menu, unsigned int num);
extern struct xurfaced_menu *xurfaced_menu_init(struct xurfaced *xurfaced);

#endif

