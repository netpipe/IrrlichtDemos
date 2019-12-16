#ifndef _GLWIDGET_H_
#define _GLWIDGET_H_

#include <QtOpenGL/qgl.h>
#include <QKeyEvent>
#include <Horde3D.h>
#include <Horde3DUtils.h>

class GLWidget : public QGLWidget{
  Q_OBJECT

  public:
	GLWidget(QWidget *parent = 0);
	~GLWidget();
	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	
  protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width,int height);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);

  private:
	ResHandle logoRes;
};

#endif // GLWIDGET_H_INCLUDED

