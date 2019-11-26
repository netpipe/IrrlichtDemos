#include "findavgcolorsinmain.h"





FindAvgColorsInMain::FindAvgColorsInMain(QString picName, int subPicWidth, int subPicHeight, int numOfSubPictures, int precision)
    :FindAverageColors(subPicWidth,subPicHeight,precision)
{

    m_pic = new QImage();
    m_numOfSubPics = numOfSubPictures;
    m_picName = picName;

    //size of the array is equal to the number of pictures in the given directory * precision * precision
    //sub array will be equal to 3 [R,G,B]
    m_averageRGBArr = new RGB [m_numOfSubPics * m_precision * m_precision];

}

/* calculates all the averages for each part in main picture,
   main picture divided to number of pictures the photo mosaic  will contain
   so if photomosaic will be have 14400 pictures, then main picture divided to 14400 parts and each part will be treated like a picture and will have its own
   RGB averages procedure is almost the same as calculateAllAverages in Directory,
   but instead of loading images from directory, program loads images from the main picture(SCALED)

   if precision > 1 then the strategy is same as calculateAllAverags in Director.

   main picture divided to m_numOfSubPictures part.
   this function also frees the memory allcated by m_pic.
   after this function m_pic won't be needed.
*/
void FindAvgColorsInMain::calculateAllAverages()
{
    QElapsedTimer timer;
    timer.start();
    //    qDebug() << m_picName;
    m_pic->load(m_picName);
    //current position of x and y in the main picture
    int currentX = 0;
    int currentY = 0;
    //number of sub pictures on X side and Y side,
    int picsOnX = m_pic->width() / m_subPicWidth;
    int picsOnY = m_pic->height() / m_subPicHeight;

    int count = 0; // 0 to num of pictures.. count++
    for (int i = 0; i < picsOnY; i++) {
        for (int j = 0; j < picsOnX; j++) {
//            QRect piece(currentX,currentY,m_subPicWidth,m_subPicHeight); //piece of a picture, QRect used instead to decrease number of arguments for createSubImage
//            QImage subImage = createSubImage(m_pic,piece);

            QImage subImage = createSubImage(m_pic,currentX,currentY,m_subPicWidth,m_subPicHeight);

            currentX += m_subPicWidth;

            if (m_precision > 1) {
                RGB *arr = higherPrecision(&subImage);
                for (int i = 0; i < m_precision * m_precision; i++) {
                    m_averageRGBArr[count + i] = arr[i];
                }
                delete arr;
                count += m_precision * m_precision;
            } else {
                m_averageRGBArr[count] = calculateAverage(&subImage);
                count++;
            }
        }
        currentX = 0;
        currentY += m_subPicHeight;
    }

    qDebug() << "allAverages() for MAIN Took" << timer.elapsed() << "milliseconds";
    delete m_pic;
}


int FindAvgColorsInMain::getArraySize() const
{
    return m_numOfSubPics * m_precision * m_precision;
}


FindAvgColorsInMain::~FindAvgColorsInMain()
{

    delete[] m_averageRGBArr;
}
