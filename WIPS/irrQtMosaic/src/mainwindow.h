#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QDebug>
#include <QString>

namespace Ui {
class MainWindow;
}


/* This is the class whicrepresents the UI, in QT, signals and slots used to carry input to other classes
 *  GUI will be in event loop  and unlike cin, GUI will not wait user to enter information it will start executing other codes,
 *  that is why signals and slots are used.
 *  Q_OBJECT macro is required in classes which have signals or slots as their members.
 *
 * checkInput checks the entered input, it calls allValidInput which returns true if all the input entered is valid
 * if not, prints a message in red color and asks user to enter valid information,
 * when all the inputs are valid, it puts all the input information in a signal that carry a string vector then sends the signal to
 * ImageInfo class.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void checkInput();
    void askSubImageDir();
    void askMainImagePath();

signals:
    void updateAll(QVector<QString> vec);

private:
    Ui::MainWindow *ui;
    bool allValidInput(int mainWidth, int mainHeight, int numSubImg, QString ratio, QString mainImgPath, QString subImgDir) const;

    int m_mainWidth;
    int m_mainHeight;
    int m_numSubImg;
    int m_subWRatio;
    int m_subHRatio;
    QVector<QString> memberVariables;

    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
