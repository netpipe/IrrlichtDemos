#include "cardgame.h"

int main()
{
    GameManager* game = new GameManager();

    if(game)
    while( game->run() )
    {
        game->drawAll();
        game->processEvents();
        game->Device()->yield();
    }

    game->endGame();

    if(game)
        return 0;
    else
        return 1;
}
