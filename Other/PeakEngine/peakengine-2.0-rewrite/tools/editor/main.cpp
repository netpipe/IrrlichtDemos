#include <qapplication.h>
#include <include/glwidget.h>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  
  GLWidget widget;
  
  widget.show();
  
  return app.exec();
  
}
