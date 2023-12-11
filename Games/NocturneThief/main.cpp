#include "src/App.h"



int main(int argc, char** argv)
{

    App* app=new App();

    //app->loadMap(argv[0]);
    app->loadMap("../media/MAP1.png");
    app->run();
    //app->startLevelEditor();


    delete app;


    return 0;
}

