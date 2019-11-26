#include "imageinfo.h"


ImageInfo::ImageInfo()
{
}

/*Find the sub-image directory and return the path. */
QString ImageInfo::askSubImageDir() const
{
    return QFileDialog::getExistingDirectory(0, "Select the directory of pictures",
                         "/home",
                         QFileDialog::ShowDirsOnly
                         | QFileDialog::DontResolveSymlinks);
}

QString ImageInfo::askMainImagePath() const
{
    return QFileDialog::getOpenFileName(0,"Select the Base picture","/home",
                    "Images (*.jpg *.jpeg *.png)");
}

/* given width, height of the main picture,  number of small pictures and ratio of those small pictures,
 * it finds the width and height of the small picture that can be put inside the master picture numOfPic times
 * if numOfPic is not appropriate (causes non-integer width or height, then numOfPic will be incremented until they are integer.
 * this method will initialize member variables: m_subImageWidth, m_subImageHeight, and m_numOfSubImages
*/
void ImageInfo::initSubResAndNum(int subImgWRatio, int subImgHRatio, int approxNumOfSubImages)
{

    double allPixels = m_mainImageWidth * m_mainImageHeight; //pixels in the big picture

    double smallPicPixel = allPixels / approxNumOfSubImages; //pixels in each small picture

    //# of pixels for each ratio constant. (pixelsForEachRatio * ratioW will give the # of pixels for width of the picture)
    double pixelsForEachRatio = sqrt(smallPicPixel / (subImgWRatio * subImgHRatio));

    //increment the number of pictures until the pixels for each ratio has no floating points.
    while ((pixelsForEachRatio != ceil(pixelsForEachRatio)) ) {
    approxNumOfSubImages++;
    smallPicPixel = allPixels / approxNumOfSubImages;
    pixelsForEachRatio = sqrt(smallPicPixel / (subImgWRatio * subImgHRatio));
//        qDebug() << approxNumOfSubImages;
    }

    m_subImageWidth = pixelsForEachRatio * subImgWRatio;
    m_subImageHeight = pixelsForEachRatio * subImgHRatio;
    m_numOfSubImages = approxNumOfSubImages;

}


/* slot which setts all the member variables.
 * vector consist of 5 parts {mainImageWidth, mainImageHeight, subImageWRatio, subImageHRatio, numOfSubImages}
 * incoming signal will be used to find subImage width, height, and precise number of images.
 * then it will update the number of images and emit that signal along with other signals for subImage width.
*/
void ImageInfo::setAll(QVector<QString> vec)
{
    qDebug() << "width: " << vec.value(0).toInt();
    qDebug() << "height: " << vec.value(1).toInt();
    qDebug() << "wRatio: " << vec.value(2).toInt();
    qDebug() << "hRatio: " << vec.value(3).toInt();
    qDebug() << "numOfImages: " << vec.value(4).toInt();

    qDebug() << "Image Path:" << vec.value(5);
    qDebug() << "Sub Image Directory" << vec.value(6);

    m_mainImageWidth = vec.value(0).toInt();
    m_mainImageHeight = vec.value(1).toInt();



    int wRatio = vec.value(2).toInt();
    int hRatio = vec.value(3).toInt();
    int approxNumOfImages = vec.value(4).toInt();

    initSubResAndNum(wRatio, hRatio, approxNumOfImages); // finds sub image resolution and num of sub images.

    qDebug() << m_subImageWidth;
    qDebug() << m_subImageHeight;
    qDebug() << m_numOfSubImages;

//    qDebug() << m_subImageWidth;
//    qDebug() << m_subImageHeight;
//    exit(1);


    emit updateSubImageWidth(m_subImageWidth);
    emit updateSubImageHeight(m_subImageHeight);

    vec[4] = QString::number(m_numOfSubImages); // updates the numOfSubImages.
    emit updateAll(vec);
}

