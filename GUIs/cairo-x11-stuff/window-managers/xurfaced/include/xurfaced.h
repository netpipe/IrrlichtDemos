#ifndef XURFACED_XURFACED_H
#define XURFACED_XURFACED_H

struct xurfaced_config
{

    char base[128];
    char head[128];
    char oninit[128];
    char onexit[128];
    char pid[128];
    char key[128];
    char notify[128];
    char notification[256];

};

struct xurfaced
{

    struct xurfaced_config config;
    struct xurfaced_display_backend *backend;
    int running;
    int paused;
    pthread_t threadRender;
    pthread_t threadEvents;
    pthread_mutex_t mutexMenu;
    pthread_mutex_t mutexRender;
    pthread_cond_t condRender;
    struct xurfaced_menu *menu;
    struct xurfaced_client_list *clients;
    int pipe[2];

};

#endif

