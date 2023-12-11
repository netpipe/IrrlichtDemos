#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <animation.h>
#include <config.h>
#include <client.h>
#include <display.h>
#include <event.h>
#include <xurfaced.h>
#include <menu.h>
#include <render.h>
#include <window.h>

struct xurfaced xurfaced;

static void *xurfaced_thread_render(void *arg)
{

    struct xurfaced *xurfaced = (struct xurfaced *)arg;
    struct timespec ts;
    struct timeval tv;

    while (xurfaced->running)
    {

        pthread_mutex_lock(&xurfaced->mutexMenu);
        xurfaced_render_prep(xurfaced->backend, xurfaced->menu);
        xurfaced_render_blit(xurfaced->backend);
        pthread_mutex_unlock(&xurfaced->mutexMenu);

        gettimeofday(&tv, 0);

        ts.tv_sec = tv.tv_sec;
        ts.tv_nsec = tv.tv_usec * 1000;
        ts.tv_nsec += 10 * 1000 * 1000;

        pthread_cond_timedwait(&xurfaced->condRender, &xurfaced->mutexRender, &ts);

    }

    return 0;

}

static void *xurfaced_thread_event(void *arg)
{

    struct xurfaced *xurfaced = (struct xurfaced *)arg;

    while (xurfaced->running)
        xurfaced_event_handler(xurfaced);

    return 0;

}

static void xurfaced_signal_term(int sig)
{

    xurfaced.running = 0;

}

static void xurfaced_signal_usr1(int sig)
{

    struct xurfaced_menu *menu = xurfaced_menu_init(&xurfaced);

    if (!menu)
        return;

    pthread_mutex_lock(&xurfaced.mutexMenu);

    if (xurfaced.menu)
        xurfaced_menu_destroy(xurfaced.menu);

    xurfaced.menu = menu;

    pthread_mutex_unlock(&xurfaced.mutexMenu);

}

void xurfaced_execute(char *command, int pipe[])
{

    char args[4096], *argv[32];

    memcpy(args, command, strlen(command) + 1);

    argv[0] = strtok(args, " ");

    unsigned int i = 0;

    while ((argv[++i] = strtok(0, " ")));

    int pid = fork();
    int pchild;

    if (pid == -1)
        return;

    if (!pid)
    {

        pchild = getpid();

        if (pipe)
        {

            close(1);
            dup(pipe[1]);
            close(pipe[0]);
            close(pipe[1]);

        }

        if (xurfaced.backend->display)
            close(xurfaced.backend->descriptor);

        setsid();
        execvp(argv[0], argv);
        exit(EXIT_FAILURE);

    }

    if (pipe)
    {

        close(pipe[1]);

        int status;

        wait(&status);

    }

}

static void xurfaced_run(struct xurfaced *xurfaced)
{

    xurfaced->running = 1;
    xurfaced->paused = 0;

    pthread_mutex_init(&xurfaced->mutexRender, 0);
    pthread_cond_init(&xurfaced->condRender, 0);
    pthread_mutex_init(&xurfaced->mutexMenu, 0);
    pthread_create(&xurfaced->threadRender, 0, xurfaced_thread_render, xurfaced);
    pthread_create(&xurfaced->threadEvents, 0, xurfaced_thread_event, xurfaced);
    pthread_join(xurfaced->threadEvents, 0);
    pthread_join(xurfaced->threadRender, 0);
    pthread_mutex_destroy(&xurfaced->mutexMenu);
    pthread_cond_destroy(&xurfaced->condRender);
    pthread_mutex_destroy(&xurfaced->mutexRender);

}

static void xurfaced_init_config(struct xurfaced_config *config)
{

    char *home = getenv("HOME");

    sprintf(config->base, "%s/.xurfaced", home);
    sprintf(config->head, "%s/head", config->base);
    sprintf(config->oninit, "%s/oninit", config->base);
    sprintf(config->onexit, "%s/onexit", config->base);
    sprintf(config->pid, "%s/pid", config->base);
    sprintf(config->key, "%s/key", config->base);
    sprintf(config->notify, "%s/notify", config->base);

}

static void xurfaced_init(struct xurfaced *xurfaced)
{

    int status;
    struct stat info;
    char copyCmd[128];

    signal(SIGTERM, xurfaced_signal_term);
    signal(SIGUSR1, xurfaced_signal_usr1);

    xurfaced_init_config(&xurfaced->config);

    if (stat(xurfaced->config.base, &info) == -1)
    {

        sprintf(copyCmd, "/bin/cp -r /usr/share/xurfaced %s", xurfaced->config.base);
        system(copyCmd);

    }

    char pid[32];

    sprintf(pid, "%u", getpid());
    xurfaced_config_write(xurfaced->config.pid, pid);
    sync();

    xurfaced->backend = xurfaced_display_create();
    xurfaced->clients = xurfaced_client_list_create();

    xurfaced_window_init(xurfaced->backend);
    xurfaced_render_init(xurfaced->backend);
    xurfaced_execute(xurfaced->config.oninit, 0);
    wait(&status);

}

static void xurfaced_destroy(struct xurfaced *xurfaced)
{

    xurfaced_menu_destroy(xurfaced->menu);
    xurfaced_client_list_destroy(xurfaced->clients);
    xurfaced_render_destroy(xurfaced->backend);
    xurfaced_window_destroy(xurfaced->backend);
    xurfaced_display_destroy(xurfaced->backend);

}

int main(int argc, char *argv[])
{

    xurfaced_init(&xurfaced);
    xurfaced_run(&xurfaced);
    xurfaced_destroy(&xurfaced);

    return 0;

}

