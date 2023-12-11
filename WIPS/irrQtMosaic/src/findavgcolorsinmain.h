#ifndef FINDAVGCOLORSINMAIN_H
#define FINDAVGCOLORSINMAIN_H

#include "findaveragecolors.h"
#include <QImage>
#include <QString>
#include <QElapsedTimer>


#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QRect>


/* This class takes a path to main picture, width and height for subpicture and number of subpictures then it divides the
 * picture to numOfSubPicture where width of each cell will be subPicWidth and the height of each cell will be subPicHeight
 * then calculates the average RGB for each given cell (unless precision > 1)
 * if precision is higher than 1 then it divides each cell to precision^2 parts then calculates average for each part.
*/
class FindAvgColorsInMain : public FindAverageColors
{
public:
    FindAvgColorsInMain(QString picName, int subPicWidth, int subPicHeight, int numOfSubPics, int precision = 1);
    void calculateAllAverages();
    int getArraySize() const;

    ~FindAvgColorsInMain();


private:
    int m_numOfSubPics;
    QString m_picName; // preferebly absolute path.
    QImage *m_pic;
};

#endif // FINDAVGCOLORSINMAIN_H
