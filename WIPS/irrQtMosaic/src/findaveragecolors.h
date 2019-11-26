#ifndef FINDAVERAGECOLORS_H
#define FINDAVERAGECOLORS_H

#include <QImage>

//Abstract Base Class for FindAveragColorsInDirectory and FindAverageColorsInMain
//This class is created to reduce the code repetition.

struct RGB
{
    double R;
    double G;
    double B;
};



/* Abstract base class for FindAvgColorsInDirectory and FindAverageColorsInMain
   both children uses getAverageRGBArr but initialize it differently in their constructors.
*/
class FindAverageColors
{

public:
    RGB* getAverageRGBArr() const;


protected:
    FindAverageColors(int subWidth, int subHeight, int precision);
    void virtual calculateAllAverages() = 0;
    int virtual getArraySize() const = 0;
    RGB  calculateAverage(QImage* pic) const;


//    QImage createSubImage(QImage* image, const QRect & rect) const;
    RGB* higherPrecision(QImage *subImage) const;



    QImage createSubImage(QImage* image, const int &xCord, const int &yCord, const int &width, const int &height) const;


    RGB* m_averageRGBArr;
    int m_subPicWidth;
    int m_subPicHeight;

    // presicion decides how many pieces the picture will be divided (by default it's 1.
    // if the picture divided to more pieces then color average will be more precise because instead of comparing 2 euclidean distance
    // program will start comparing m_presicion euclidean distances between pictures
    // ex: m_presicion = 9, then each sub picture divided to 3 row and 3 col and each piece compared to corresponding piece from main picture
    //(of course piece from main picture also divided to 9 pieces) (piece1 of a piece from main picture compared to piece1 from subpicture)
    // now there will be 9 more comparison, and avgarray will be 9 times bigger.
    int m_precision;

    virtual ~FindAverageColors() {}

};

#endif // FINDAVERAGECOLORS_H
