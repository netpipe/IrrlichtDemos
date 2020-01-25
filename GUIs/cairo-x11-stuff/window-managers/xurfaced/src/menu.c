#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <animation.h>
#include <config.h>
#include <display.h>
#include <xurfaced.h>
#include <menu.h>

struct xurfaced_menu_option *xurfaced_menu_option_create()
{

    struct xurfaced_menu_option *option = malloc(sizeof (struct xurfaced_menu_option));
    option->name = 0;
    option->description = 0;
    option->command = 0;
    option->animationProperties.alpha = 0;
    option->next = 0;
    option->prev = 0;

    return option;

}

void xurfaced_menu_option_destroy(struct xurfaced_menu_option *option)
{

    free(option->name);
    free(option->description);
    free(option->command);
    free(option);

}

struct xurfaced_menu_option_list *xurfaced_menu_option_list_create()
{

    struct xurfaced_menu_option_list *list = malloc(sizeof (struct xurfaced_menu_option_list));
    list->head = 0;
    list->current = 0;

    return list;

}

void xurfaced_menu_option_list_destroy(struct xurfaced_menu_option_list *list)
{

    struct xurfaced_menu_option *current = list->head;

    do
    {

        struct xurfaced_menu_option *next = current->next;

        xurfaced_menu_option_destroy(current);

        current = next;

    }
    while (current != list->head);

    list->current = 0;

}

void xurfaced_menu_option_list_add(struct xurfaced_menu_option_list *list, struct xurfaced_menu_option *option)
{

    if (!list->head)
    {

        option->next = option;
        option->prev = option;

        list->head = option;

    }

    else
    {

        option->next = list->head;
        option->prev = list->head->prev;

        list->head->prev->next = option;
        list->head->prev = option;

    }

}

void xurfaced_menu_option_list_remove(struct xurfaced_menu_option_list *list, struct xurfaced_menu_option *option)
{

    if (option == list->head)
        list->head = option->next;

    option->prev->next = option->next;
    option->next->prev = option->prev;

    if (option == option->next)
        list->head = 0;

}

struct xurfaced_menu *xurfaced_menu_create()
{

    struct xurfaced_menu *menu = malloc(sizeof (struct xurfaced_menu));
    menu->opts = xurfaced_menu_option_list_create();

    return menu;

}

void xurfaced_menu_destroy(struct xurfaced_menu *menu)
{

    xurfaced_menu_option_list_destroy(menu->opts);
    free(menu->opts);
    free(menu);

}

void xurfaced_menu_activate(struct xurfaced_menu *menu)
{

    if (strlen(menu->opts->current->command))
        xurfaced_execute(menu->opts->current->command, 0);

}

void xurfaced_menu_next(struct xurfaced_menu *menu, unsigned int num)
{

    struct xurfaced_menu_option *option = menu->opts->current;

    if (option == menu->opts->head->prev)
        return;

    while ((option = option->next))
    {

        if (!strlen(option->name))
            continue;

        if (!--num)
            break;

        if (option == menu->opts->head->prev)
            break;

    }

    menu->opts->current = option;

}

void xurfaced_menu_previous(struct xurfaced_menu *menu, unsigned int num)
{

    struct xurfaced_menu_option *option = menu->opts->current;

    if (option == menu->opts->head)
        return;

    while ((option = option->prev))
    {

        if (!strlen(option->name))
            continue;

        if (!--num)
            break;

        if (option == menu->opts->head)
            break;

    }

    menu->opts->current = option;

}

static FILE *xurfaced_open(struct xurfaced *xurfaced, char *head, char *title)
{

    char path[128];
    struct stat info;

    sprintf(path, "%s/%s/%s", xurfaced->config.base, head, title);
    stat(path, &info);

    if (info.st_mode & S_IXUSR)
    {

        pipe(xurfaced->pipe);
        xurfaced_execute(path, xurfaced->pipe);

        return fdopen(xurfaced->pipe[0], "r");

    }
    
    else
    {

        return fopen(path, "r");

    }

}

static void xurfaced_menu_init_options(struct xurfaced *xurfaced, struct xurfaced_menu *menu, char *path)
{

    FILE *title = xurfaced_open(xurfaced, path, "title");
    FILE *desc = xurfaced_open(xurfaced, path, "desc");
    FILE *exec = xurfaced_open(xurfaced, path, "exec");

    if (!title || !desc || !exec)
        return;

    float y = 0;
    char content[4096];
    struct xurfaced_menu_option *option;

    for (;;)
    {

        if (!fgets(content, 4096, title))
            break;

        content[strlen(content) - 1] = '\0';

        option = xurfaced_menu_option_create();
        option->name = 0;
        option->description = 0;
        option->command = 0;

        option->name = malloc(strlen(content) + 1);

        strcpy(option->name, content);
        
        if (fgets(content, 4096, desc) != 0)
        {

            content[strlen(content) - 1] = '\0';
            option->description = malloc(strlen(content) + 1);

            strcpy(option->description, content);

        }

        if (fgets(content, 4096, exec) != 0)
        {

            content[strlen(content) - 1] = '\0';
            option->command = malloc(strlen(content) + 1);

            strcpy(option->command, content);

        }

        option->animationProperties.translationX = 0;
        option->animationProperties.translationY = y;
        y += xurfaced->backend->height / 10.0;

        xurfaced_menu_option_list_add(menu->opts, option);

    }

    fclose(title);
    fclose(desc);
    fclose(exec);


}

struct xurfaced_menu *xurfaced_menu_init(struct xurfaced *xurfaced)
{

    struct xurfaced_menu *menu = xurfaced_menu_create();
    menu->animationProperties.translationX = xurfaced->backend->width / 16.0;
    menu->animationProperties.translationY = xurfaced->backend->height / 4.0 + xurfaced->backend->height / 8.0;

    char path[128];

    xurfaced_config_read(xurfaced->config.head, path, 128);

    path[strlen(path) - 1] = '\0';

    xurfaced_menu_init_options(xurfaced, menu, path);

    menu->opts->current = menu->opts->head;

    return menu;

}

