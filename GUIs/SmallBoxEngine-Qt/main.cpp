#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QVBoxLayout>
#include "smallboxWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;

    smallBoxWidget renderer;

    QWidget* container = window.GetContainer();
    QVBoxLayout* layout = (QVBoxLayout*)container->layout();
    layout->addWidget(&renderer);

    window.show();

    renderer.init(container);

        // Make sure the Irrlicht Device exists before trying to use it
        if(renderer.getIrrlichtDevice())
        {
            ISceneManager *smgr = renderer.getIrrlichtDevice()->getSceneManager();
            IVideoDriver *driver = renderer.getIrrlichtDevice()->getVideoDriver();

            // Just display a simple mesh
            IAnimatedMesh* mesh = smgr->getMesh("media/sydney.md2");
            if (!mesh)
            {
                renderer.getIrrlichtDevice()->drop();
                return 1;
            }

            IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
            if (node)
            {
                node->setMaterialFlag(EMF_LIGHTING, false);
                node->setMD2Animation(scene::EMAT_STAND);
                node->setMaterialTexture( 0, driver->getTexture("media/sydney.bmp") );
            }
        }

    return app.exec();
}
