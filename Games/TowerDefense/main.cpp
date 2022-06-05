#include "CApplication.h"
#include "GameVariables.h"

int main(void)
{
    CApplication app;
    app.init(game_width*tile_size,game_height*tile_size);
    app.run();

    return 0;
}
