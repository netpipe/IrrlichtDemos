#include "findaveragecolors.h"

#include <QDebug>

FindAverageColors::FindAverageColors(int subWidth, int subHeight, int precision)
{
    m_subPicWidth = subWidth;
    m_subPicHeight = subHeight;
    m_precision = precision;

}


RGB *FindAverageColors::getAverageRGBArr() const
{
    return m_averageRGBArr;
}




/* Calculates the average RGB in each picture, this function is a helper function for calculateAllAverages() in FindAvgColorsInMain or FindAvgColorsInDirectory
 * function takes a picture, then it reads every R,G,B value for each pixel in the picture, it then sums the values seperately.
 * (sumR for all sums of R values, sumG for G values...) Then finally takes the mean of each sum.
 * function creates RGB structure, initializes average R,G,B values then returns the struct.
*/

RGB FindAverageColors::calculateAverage(QImage* pic) const
{

    // will contain sum of all R,G,B colors in every pixel in given image.
    double sumR = 0;
    double sumG = 0;
    double sumB = 0;

    int x =  pic->width();
    int y = pic->height();
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            QRgb pix = pic->pixel(i,j);
            sumR += qRed(pix);
            sumG += qGreen(pix);
            sumB += qBlue(pix);
        }
    }
    RGB avgRGB;
    avgRGB.R = sumR / (x * y);
    avgRGB.G = sumG / (x * y);
    avgRGB.B = sumB / (x * y);
    return avgRGB;

}

/* Usually in photo mosaics 2 techniques used to select an image, either check the specified area and take the mean then find the most similar image in library
 * OR check every pixel in the specified area then find the most similar image(by performing euclidean distance for every pixel).
 * First method will be fast but the result won't be astheatically pleaseing, and dithering will be needed to reduce the same picture to appear in given area
 * Second method is very slow but the result will be very pleasing it, and in most cases dithering wont be needed since the algorithm will select different images.
 *
 * So first one produces very fast but very inaccurate results and second one produces very slow but very accurate results,
 * I decided to create an algorithm that uses both, instead of checking every pixel, I introduced a concept of precision, if the precision is N
 * then every small picture in library and every cell in main picture is divided to NxN parts when comparing image from library and a cell from main picture,
 * each divided area compared to the corresponded area, this way it. This way the program will produce very accurate and relatively fast result.
*/

/* mem should be freed in calculateAllAverages.
 * takes the subImage and then divides the image into (m_precision * m_precision) parts (if m_precision = 3, then 3x3 parts)
 * then it calls calculate Average to calculateAverage of each part,
 * it returns an array of RGB structures.
 * this is done to increase the quality of the Photo Mosaic image,
*/

RGB* FindAverageColors::higherPrecision(QImage *subImage) const
{
    int currentX = 0;
    int currentY = 0;
    int widthInterval = subImage->width() / m_precision;
    int heightInterval = subImage->height() / m_precision;

    RGB *arr = new RGB[m_precision * m_precision];
    int count = 0;
    for (int i = 0; i < m_precision; i++) {
        for (int j = 0; j < m_precision; j++) {
//            QRect piece(currentX, currentY,widthInterval,heightInterval);
//            QImage subSubImage = createSubImage(subImage,piece);

            QImage subSubImage = createSubImage(subImage,currentX,currentY,widthInterval,heightInterval);

            currentX += widthInterval;
            arr[count] = calculateAverage(&subSubImage);
            count++;
        }
        currentX = 0;
        currentY += heightInterval;
    }
    return arr;
}





/*
 * takes an image and a rectangle  and takes creates a new QImage object which has the same width as m_subImageWidth and same height as m_subImageHeight,
 * takes x and y coordinates which will be the starting point of the new image. (m_subImageWidth and m_subImageHeight will be the end points.)
 *
 * function is quite fast since it takes a pointer to image which is dynamically allocated, and reads the raw data, Also bits() doesn't perform deep copy.
 * depth of image divided by 8 since depth returns BITS per pixel (bpp) but offset is specified in BYTES
 *   so the program assumes it will work on an architecture where 8-bit = 1 byte.
 *
 * QImage(const uchar *data, int width, int height, int bytesPerLine, Format format) constructer used
 * size_t used since its platform independent and stores everything that is theoratically possible. (values the memory can hold)
 * if offset requires 8terabyte in a system with 10terabyte free memory, size_t can hold it (hopefully)
*/

QImage FindAverageColors::createSubImage(QImage* image, const int &xCord, const int &yCord, const int &width, const int &height) const
{
    size_t offset = xCord * image->depth() / 8 + yCord * image->bytesPerLine();

    return QImage(image->bits() + offset, width, height, image->bytesPerLine(), image->format());

}
