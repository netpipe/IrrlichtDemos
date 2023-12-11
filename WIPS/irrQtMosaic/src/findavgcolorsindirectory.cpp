#include "findavgcolorsindirectory.h"



FindAvgColorsInDirectory::FindAvgColorsInDirectory(QDir dir, int subPicWidth, int subPicHeight, QString format, int precision)
    : FindAverageColors(subPicWidth, subPicHeight,precision)
{
    dir.setSorting(QDir::Name);
    m_directory = dir;
//    qDebug() << m_directory.path();
    m_format = format;
    m_pic = new QImage();
    m_directory.setSorting(QDir::Name);

    QStringList filter;
    filter << QString("*.") + format << QString("*.") + "jpg";
    m_directory.setNameFilters(filter);

    //size of the array is equal to the number of pictures in the given directory * (m_recision)^2,
    m_arrSize = m_directory.entryInfoList().size() * m_precision * m_precision;

    qDebug() << "arrSize: " << m_arrSize;
    qDebug() << "path: " <<         m_directory.path();
    qDebug() << "input:" << QDir::separator ();

//    changeAllRes();     //to adjust the resolution of all the images in given directory, use this. only start this when you're sure that images are not in

    m_averageRGBArr = new RGB[m_arrSize];

}

/* calculates averages for each picture in the directory and each average will be in rgbArray,
 * pictures are named numerically, so first picture is 0.jpeg, and last picture is totalPictures in directory -1.jpeg
 * this way we don't need to have an extra array to keep track of the names of the pictures.
 * m_averageRGBArr[N] will contain the average RGB values for N.jpeg
 *
 * for every picture in directory load it onto m_pic, then pass m_pic to calculateAverage (if precision == 1) then store the values for RGB in the array.
 * if precision > 1 then use higherPrecision function to calculate the precision the array size will be m_precision ^ 2 bigger,
 * so for m_averageRGBArr[0] will contain the average RGB values for the first area of 0.jpeg (since each picture is divided to m_precision^2 parts)
 * the information belongs to 0.jpeg will be in m_averateRGBArr[0] to m_averageRGBArr[m_precision^2 - 1]
 * its implemented this way to avoid using two seperate structures.
*/
void FindAvgColorsInDirectory::calculateAllAverages()
{
    qDebug() << "Starting calculateAllAverages for DIR";
    QElapsedTimer timer;
    timer.start();
    changeAllRes();

    QFileInfoList infoList= m_directory.entryInfoList();

    int pos = 0; // will eventually contain the pos for the image 2321.jpeg will be inserted into m_averageRGB[2321*(m_preciison^2)]

    int count = 0;
    int size = infoList.size();
    for (int i = 0; i < size; i++) {
    m_pic->load(infoList.at(i).filePath());

    // name of the jpeg file is in string, so take the string, get rid of everything after "." (.jpeg) rest will be the name.
    // if 0.jpeg  get rid of .jpeg, then convert rest to int that will be position.

//    pos = i;
//    checkResAndName(infoList.at(i).filePath(),pos);

    pos = infoList.at(i).fileName().lastIndexOf('.');
    pos = m_directory.entryInfoList().at(i).fileName().left(pos).toInt();

    if (m_precision > 1) {
        pos = pos * m_precision * m_precision; // necessary to jump between pos
        RGB *arr = higherPrecision(m_pic);

        for (int i = 0; i < (m_precision * m_precision); i++) {
        m_averageRGBArr[pos + i] = arr[i];
        }
        delete arr;
    } else {

        m_averageRGBArr[pos] = FindAverageColors::calculateAverage(m_pic);
    }
    count++;
    }
    qDebug() << "allAverages() for DIR Took" << timer.elapsed() << "milliseconds";
}



/* Changes the resolution of all the images in m_directory to m_subPicWidth and m_subPicHeight
 * This function is used in case if the images in the directory doesn't have the same size as m_subPicWidth and m_subPicHeight.
*/
void FindAvgColorsInDirectory::changeAllRes()
{
    QElapsedTimer timer;
    timer.start();

    qDebug() << "Changing the resolution of all images";
    qDebug() << m_subPicWidth;
    qDebug() << m_subPicHeight;

    int count = 0; // count the number of pictures that needed to be resized.
    QFileInfoList infoList= m_directory.entryInfoList();
    int size = infoList.size();

    for (int i = 0; i < size; i++) {
    m_pic->load(infoList.at(i).filePath());

    if(m_pic->width() != m_subPicWidth || m_pic->height() != m_subPicHeight) {
        *m_pic = m_pic->scaled(m_subPicWidth,m_subPicHeight);
        m_pic->save(infoList.at(i).filePath(),0,100);
        count++;
    }

    }

    if (count == 0) {
        qDebug() << "No Images resized";
    } else {
        qDebug() << count << "Images resized.";
    }
    qDebug() << "changeAllRes() Took" << timer.elapsed() << "milliseconds";

}


int FindAvgColorsInDirectory::getArraySize() const
{
    return m_arrSize;
}


FindAvgColorsInDirectory::~FindAvgColorsInDirectory()
{
    delete[] m_averageRGBArr;

    delete m_pic;
}
