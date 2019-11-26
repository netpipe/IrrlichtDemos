#ifndef FINDAVGCOLORSINDIRECTORY_H
#define FINDAVGCOLORSINDIRECTORY_H


#include "findaveragecolors.h"
#include <QDir>
#include <QString>
#include <QDebug>
#include <QElapsedTimer>
#include <QDirIterator>




/* Child class of FindAverageColors, calculates the average colors for every image in given directory.
 * Performance of this function wildy differs! in ext4 (7200rpm HDD) file formats it performs very well:
 * reading and calculating all averages of 50,000 images where precision = 4, (so reading 50,00 images and calculating 800,000 averageImage) takes 5 second.
 * However if the images are located in external disks or if file format is NTFS then there will be significant decrease in the performance.
 * and there is no precise timing, sometime it takes over 70 seconds or 10 seconds,
 * Speed is of this class is depends on the HDD, when .exe and pictures are in  SSD, above configuration takes couple of seconds.
 * for above configuration, calculating averages usually takes 1-2 seconds on intel i5, rest is to read data from hdd,
 *
 * TODO: Find a better way of changing resolution of every pic in a directory (maybe ask the user if the pictures are all have the same size, instead of checking every single image)
*/


class FindAvgColorsInDirectory : public FindAverageColors
{

public:
    FindAvgColorsInDirectory(QDir dir, int subPicWidth, int subPicHeight, QString format, int precision = 1);
    void calculateAllAverages();
    int getArraySize() const;
    virtual ~FindAvgColorsInDirectory();



private:
    void changeAllRes();
    void checkResAndName(const QString &path, const int &count);
    QDir m_directory;
    QString m_format;
    QImage *m_pic;
    int m_arrSize;

};

#endif // FINDAVGCOLORSINDIRECTORY_H
