#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


 #include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

 private slots:
    void handleButton();
 private:
    QPushButton *m_button;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
