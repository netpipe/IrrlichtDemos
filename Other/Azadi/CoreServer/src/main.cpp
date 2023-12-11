#include "main.h"

Logger log;

int main(int argc, char **argv)
{
    // Start the initializing stages
    log.initialize();

    // Start the main loop
    mainLoop();

    // stop the logger
    log.shutdown();

    return 0;
}
