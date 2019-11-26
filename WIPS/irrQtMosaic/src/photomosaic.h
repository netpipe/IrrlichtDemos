#ifndef PHOTOMOSAIC_H
#define PHOTOMOSAIC_H

#include "findavgcolorsinmain.h"
#include "findavgcolorsindirectory.h"

#include <cmath>
#include <QImage>
#include <QPainter>
#include <QObject>
#include <QDir>
#include <QtConcurrent/QtConcurrentRun> // for seperating GUI thread.
#include <QFuture>
#include <QFutureWatcher>


/* This is the main class which does all the calculations and produces the photo mosaic image.
 * runConcurrent function paralelly executes 2 main computation required before constructing photomosaic image.
 * Once the average colors found for each image in a sub directory and each cell in the main image,
 * it finds the most similar mosaics for each cells using euclideanDistanceRGB, euclidean distance between average RGB values.
 * Then finally it constructs the photomosaic image using constructImage function.
*/

class PhotoMosaic : public QObject
{
    Q_OBJECT
public:
    PhotoMosaic();

public slots:
    void getSubImageWidth(int width);
    void getSubImageHeight(int height);

    void runConcurrent(QVector<QString> vec);


private:

    void run(QVector<QString> vec);

    QString resizeImage(QString imagePath, int x, int y) const;
    void constructImage(QStringList* subPics, QImage *destImage, QString subPicDir) const;

    double euclideanDistanceRGB(RGB r1, RGB r2) const;
    QStringList* colorDifference(RGB* dirArr, int dirArrSize, RGB* mainArr, int mainArrSize, int precision = 1) const;

    FindAvgColorsInDirectory *avgDir;
    FindAvgColorsInMain *avgMain;
    int m_subImageWidth;
    int m_subImageHeight;


};

#endif // PHOTOMOSAIC_H
