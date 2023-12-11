#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowTitle("Photo Mosaic");
    scene = new QGraphicsScene(this);
    //width height subImg wRatio hRatio

    ui->setupUi(this);
    ui->precisionBox->setRange(1,20);

    //connect necessary signals to slots.
    connect(ui->getImagePath, SIGNAL(clicked()),this,SLOT(askMainImagePath()));
    connect(ui->getSubImgDir, SIGNAL(clicked()),this,SLOT(askSubImageDir()));
    connect(ui->step2, SIGNAL(clicked()),this, SLOT(checkInput()));

}


void MainWindow::askSubImageDir()
{
    QString path = QFileDialog::getExistingDirectory(0, "Select the directory of pictures",
                                                     "/home",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    ui->subImgDir->clear();
    ui->subImgDir->insert(path);
}

void MainWindow::askMainImagePath()
{
    QString path = QFileDialog::getOpenFileName(0,"Select the Base picture","/home",
                                                "Images (*.jpg *.jpeg *.png)");

    ui->imagePath->clear();
    ui->imagePath->insert(path);
}


bool MainWindow::allValidInput(int mainWidth, int mainHeight, int numSubImg, QString ratio, QString mainImgPath, QString subImgDir) const
{
    int wRatio = ratio.split(":").value(0).toInt();
    int hRatio = ratio.split(":").value(1).toInt();

    bool res = true;
    if (mainImgPath.isEmpty()) {

        ui->imagePath->clear();
        //ui->imagePath->setStyleSheet("color: red");
        //ui->imagePath->insert("Select An Image");
        res = false;
    }
    if (subImgDir.isEmpty()) {
        ui->pmWidth->clear();
        //ui->subImgDir->setStyleSheet("color: red");
        //ui->subImgDir->insert("Select Directory");
        res = false;

    }

    if(ratio.size()  < 3) {
        ui->subRatio->insert("Please Enter A Valid Aspect Ratio (X:Y)");
    }
    if (mainWidth <= 0) {
        ui->pmWidth->clear();
        ui->pmWidth->setStyleSheet("color: red");
        ui->pmWidth->insert("Enter A Valid Width (>0)");
        res = false;

    }
    if (mainHeight <= 0) {
        ui->pmHeight->clear();
        ui->pmHeight->setStyleSheet("color: red");
        ui->pmHeight->insert("Enter A Valid Height (>0)");
        res = false;


    }
    if (numSubImg <= 0) {
        ui->numOfSub->clear();
        ui->numOfSub->setStyleSheet("color: red");
        ui->numOfSub->insert("Enter A Valid Height (>0)");
        res = false;

    }
    if (wRatio <= 0 || hRatio <= 0) {
        ui->subRatio->clear();
        ui->subRatio->setStyleSheet("color: red");
        ui->subRatio->insert("Enter A Valid Ratio (X:Y)");
        res = false;

    }
    return res;
}

void MainWindow::checkInput()
{
    /*
    //DELETE THIS PART!
    ui->imagePath->insert("D:/WORKSPACE/PHOTO MOSAIC/StarWars.jpg");
    ui->subImgDir->insert("D:/WORKSPACE/PHOTO MOSAIC/PICTURES/StarWars");
    ui->pmWidth->insert("15360"); //15360
    ui->pmHeight->insert("8640"); //8640
    ui->numOfSub->insert("57600"); //57600
    ui->subRatio->insert("16:9"); //16:9
    ui->precisionBox->setValue(2); // precision
    */

    QString mainImgPath = ui->imagePath->text();
    QString subImgDir = ui->subImgDir->text();
    if (subImgDir.length() >= 1) {
        if (subImgDir.at(subImgDir.size() - 1) != '/') {
            subImgDir.insert(subImgDir.size(), '/');
        }
    }


    int precision = ui->precisionBox ->value();
    int mainWidth = ui->pmWidth->text().toInt();
    int mainHeight = ui->pmHeight->text().toInt();
    int numSubImg = ui->numOfSub->text().toInt();
    QString ratio = ui->subRatio->text();

    int wRatio = ratio.split(":").value(0).toInt();
    int hRatio = ratio.split(":").value(1).toInt();


    if (allValidInput(mainWidth,mainHeight,numSubImg,ratio,mainImgPath,subImgDir)) {

        ui->graphicsView->setScene(scene);
        QPixmap img(mainImgPath);
        scene->addPixmap(img);
//                ui->graphicsView->fitInView(0,00);
        ui->graphicsView->fitInView(scene->itemsBoundingRect() ,Qt::KeepAspectRatio);

        memberVariables.append(QString::number(mainWidth));
        memberVariables.append(QString::number(mainHeight));
        memberVariables.append(QString::number(wRatio));
        memberVariables.append(QString::number(hRatio));
        memberVariables.append(QString::number(numSubImg));
        memberVariables.append(mainImgPath);
        memberVariables.append(subImgDir);
        memberVariables.append(QString::number(precision));

        qDebug() << wRatio << hRatio;
        emit updateAll(memberVariables); //emits the signal

        // inputs are read-only.
        ui->pmWidth->setEnabled(false);
        ui->pmHeight->setEnabled(false);
        ui->subRatio->setEnabled(false);
        ui->numOfSub->setEnabled(false);
        ui->imagePath->setEnabled(false);
        ui->subImgDir->setEnabled(false);
        ui->precisionBox->setEnabled(false);

        qDebug() << "EMMITED;";
    }

}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
}
