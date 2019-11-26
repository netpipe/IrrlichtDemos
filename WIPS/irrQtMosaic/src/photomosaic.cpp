#include "photomosaic.h"

PhotoMosaic::PhotoMosaic()
{
    m_subImageWidth = 0;
    m_subImageHeight = 0;


}

void PhotoMosaic::getSubImageWidth(int width)
{
    m_subImageWidth = width;
}

void PhotoMosaic::getSubImageHeight(int height)
{
    m_subImageHeight = height;
}



/* Very easy and straight forward Qt class to distribute the computation (using multiple threads)
 * this is needed because by default Qt GUI will use the main thread (GUI Thread)
 * if there is a heavy computation then GUI will become unresponsive, so Qt requires a worker thread for heavy computations.
 * QtConcurrent deployed to create the worker thread.
 *
*/
void PhotoMosaic::runConcurrent(QVector<QString> vec)
{
    QtConcurrent::run(this,&PhotoMosaic::run,vec);
}


// This function calls all the necassary functions to generate the photomosaic image.
/*
updated vector sent from imageInfo,
  vec.value(0) contains mainWidth
  vec.value(1) contains mainHeight
  vec.value(2) and 3 no longer needed but still kept to make it consistent among classes
  vec.value(4) contains number of images will be in photo mosaic.
  vec.value(5) contains the absolute path of main Image.
  vec.value(6) contains the absolute path of the directory contains all the sub images.
  vec.value(7) contains the precision.
*/

void PhotoMosaic::run(QVector<QString> vec)
{

    QString subImageDir = vec.value(6);
    QString mainImagePath = vec.value(5);

    int mainWidth = vec.value(0).toInt();
    int mainHeight = vec.value(1).toInt();

    int subWidth = m_subImageWidth;
    int subHeight = m_subImageHeight;
    int numOfSubImages = vec.value(4).toInt();


    mainImagePath = resizeImage(mainImagePath,mainWidth,mainHeight);

    QDir directory(subImageDir);



    int precision = vec.value(7).toInt();

    FindAvgColorsInDirectory *avgDir = new FindAvgColorsInDirectory(directory, subWidth, subHeight, "jpeg",precision);
    FindAvgColorsInMain *avgMain = new FindAvgColorsInMain(mainImagePath,subWidth,subHeight,numOfSubImages,precision);

    //QFuture carries the result of a running thread.
    QFuture<void> avgDirThread =  QtConcurrent::run(avgDir,&FindAvgColorsInDirectory::calculateAllAverages);
    QFuture<void> avgMainThread = QtConcurrent::run(avgMain,&FindAvgColorsInMain::calculateAllAverages);


    qDebug() << "waiting to finish threads";
    //wait until both threads finished.
    avgMainThread.waitForFinished();
    avgDirThread.waitForFinished();


    RGB *dirStruct = avgDir->getAverageRGBArr();
    RGB *mainStruct = avgMain->getAverageRGBArr();


    //This is how it could be computed without using extra threads.
    //    FindAvgColorsInDirectory *avgDir = new FindAvgColorsInDirectory(directory, subWidth, subHeight, "jpeg",precision);
    //    avgDir->calculateAllAverages();
    //    RGB *dirStruct = avgDir->getAverageRGBArr();

    //    FindAvgColorsInMain *avgMain = new FindAvgColorsInMain(mainImagePath,subWidth,subHeight,numOfSubImages,precision);
    //    avgMain->calculateAllAverages();
    //    RGB *mainStruct = avgMain->getAverageRGBArr();


    QStringList *listRepeat = colorDifference(dirStruct, avgDir->getArraySize(), mainStruct, avgMain->getArraySize(),(precision * precision));


    QImage base;
    base.load(mainImagePath);
    QImage* img2 = new QImage(mainWidth,mainHeight, base.format());



    constructImage(listRepeat,img2, subImageDir);
    QString saveName = QString("%1(%2)%3")
            .arg("Photo Mosaic RGB ")
            .arg(QString::number(precision))
            .arg(".jpg");

    qDebug() << saveName;
    img2->save(saveName);

    //free unused memmory.
    delete listRepeat;
    delete img2;
    delete avgMain;
    delete avgDir;
}

//finds the euclidean distance between 2 pixels using their RGB values: r1,g1,b1 and r2,g2,b2
double PhotoMosaic::euclideanDistanceRGB(RGB RGB1, RGB RGB2) const
{

    double r = pow((RGB1.R - RGB2.R),2);
    double g = pow((RGB1.G - RGB2.G),2);
    double b = pow((RGB1.B - RGB2.B),2);


    double res = r + g + b;
    return sqrt(res);

}

//use function pointers, it takes a function as its argument and uses the function to calculate the color difference,
//that way no need for if and else
//takes another function to find which euclideanfunction it uses(RGB or HSV)

/* Once all the average RGB values found for both sub-images and each block in the main image,
 * this function finds the color difference between
*/
QStringList* PhotoMosaic::colorDifference(RGB* dirArr, int dirArrSize, RGB* mainArr, int mainArrSize, int precision) const
{
    qDebug() << "in colordifference";
    QStringList *list = new QStringList(); // size of i,

    QElapsedTimer timer;
    timer.start();
    int pos = 0;

    int percentageDone = 0;
    int onePercent = mainArrSize / 100;
    int count = onePercent;
    qDebug() << onePercent;
    for (int i = 0; i < mainArrSize; i = i + precision) {
        // display percentage of the work is done.
        if (i >= count) {
            percentageDone++;
            count += onePercent;
            qDebug() << percentageDone;
        }

        //Calculate the average euclidean distances for each picture in the sub-picture directory.
        //based on the precision, divide each block further by precision.
        //this can theoratically increase the accuracy (as long as there are enough mosaics to choose from.
        double min = 255 * precision * precision; // it should be 255 * (precision * precision)
        for (int j = 0; j < dirArrSize; j = j + precision) {
            double sum = 0;
            for (int x = 0; x < precision; x++) {
                sum += euclideanDistanceRGB(mainArr[i + x], dirArr[j + x]);
            }

            if (sum < min) {
                min = sum;
                pos = j / precision;
            }
            sum = 0;
        }
        QString picName = QString::number(pos);
        list->append(picName);
    }

    qDebug() << "COLOR DIFFERENCE Took" << timer.elapsed() << "milliseconds";
    return list;
}


/*resizes the image if it has different width or height then given input. */
QString PhotoMosaic::resizeImage(QString imagePath, int x, int y) const
{
    QImage image;
    image.load(imagePath);

    if(image.width() == x && image.height() == y) {
        qDebug() << "no need to resize";
        return imagePath;
    }

    QImage newImage = image.scaled(x,y);
    imagePath = imagePath.remove(".jpg");
    QString name = imagePath  + " SCALED" + ".jpg";
    newImage.save(name,0,100);
    return name;
}


/* Takes a list of strings consist of the path of the mosaics to be used to construct the photomosaic, empty QImage with same width and height as the photomosaic image.
 * It then construct the image by using the images subPics points to.
 * Ex: subPics(0) will represent the mosaic in very top left corner of the image.
*/

void PhotoMosaic::constructImage(QStringList* subPics, QImage *destImage, QString subPicDir) const
{
    QElapsedTimer timer;
    timer.start();

    QImage img;
    int currentX = 0;
    int currentY = 0;
    QPoint pos;
    QPainter painter(destImage);

    img.load(subPicDir + subPics->at(0) + ".jpeg");

    int picsOnX = destImage->width() / img.width();
    int picsOnY = destImage->height() / img.height();

    int count = 0;
    for (int i = 0; i < picsOnY; i++) {
        for (int j = 0; j < picsOnX; j++) {
            img.load(subPicDir + subPics->at(count) + ".jpeg");
            pos = QPoint(currentX,currentY);
            painter.drawImage(pos,img);
            currentX += img.width();
            count++;
        }
        currentX = 0;
        currentY += img.height();
    }
    qDebug() << "CONSTRUCT IMAGE Took" << timer.elapsed() << "milliseconds";
}
