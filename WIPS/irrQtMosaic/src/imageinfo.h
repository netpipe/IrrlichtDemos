#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <math.h>
#include <QDebug>
//#include <QFileDialog>
#include <QString>
#include <QObject>


/* This class gathers various info about both base image and sub images (mosaics) from the user
 * It uses the info to calculate ratios for the photomosaic image, scale the image if necessary,
 * then emits all the info to classes that requires them.
 *
 * Image Info for base image:
 * 1) Width
 * 2) Height
 * 3) Image Path
 * 4) Precision: How precise the photomosaic image should be (Experimental)
 * If the main image is not scaled to the desired width and height, program will automatically scale it.
 *
 * Image Info for sub-images:
 * 1) Number Of Images in the sub-image directory
 * 2) Height Ratio
 * 3) Width Ratio
 * 4) Image Path
 *
*/


class ImageInfo : public QObject
{
    Q_OBJECT
public:
    ImageInfo();

    QString askSubImageDir() const;
    QString askMainImagePath() const;


public slots:
    void setAll(QVector<QString> vec);

signals:
    void updateSubImageWidth(int width);
    void updateSubImageHeight(int height);
    void updateNumOfSubImages(int numOfImg);
    void updateAll(QVector<QString> vec);


private:
    void initSubResAndNum(int subImgWRatio, int subImgHRatio, int approxNumOfSubImages);
    int m_mainImageWidth;
    int m_mainImageHeight;

    int m_subImageWidth;
    int m_subImageHeight;

    int m_numOfSubImages;




};

#endif // IMAGEINFO_H
