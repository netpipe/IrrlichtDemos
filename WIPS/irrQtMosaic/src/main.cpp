/*  Photo Mosaic Creator
    Written By Onur Demiralay 17/12/2012
    odemiral@sfu.ca
*/

#include <QApplication>
#include <QDebug>

#include "imageinfo.h"
#include "photomosaic.h"
#include "mainwindow.h"
#include "grabvideoframes.h"


/*TODO:
 * 1) Algorithm to find photomosaics is brute-force (O(N^2)) therefore very inefficient. Try to come up with a smarter way of finding mosaics.
 * Edit: I think range trees would be perfect for this job (https://www.cise.ufl.edu/class/cot5520fa13/CG_RangeTrees.pdf)
 * 3D range tree where each dimension represents a color channel (R,G,B) instead of going through all the possible images for each cell,
 * I'd only need to compare it with all the images within the range of range tree.
 * Construction would take O(nlog^2(n))
 * Range query with fractional cascading would take: O(log^2(n) +k) where k is the number of results in range.
 * Pros: Very Efficient, Sounds very cool to implement.
 * Cons: I need to implement a balanced tree(r&b tree would efficient for this) for each leaf.
 * 2) It might be a better idea to store all the sub-images in given directory into an internal DB instead of reading from HDD every single time.
 * it's very inefficient if the directory is located in an external hdd or usb.
 * 3) Find a way to optimize your code further
 *      a. RGB should be powof2 to increase the likelihood of getting super-aligned in memory.
 *      b. Measure your cache miss ratio using valgrind to see if it's considerably high.
 * 4) Make the photomosaic work with uneven images
 *      a. Crop them to make it fit.
 *      b. or automatically resize the photomosaic image size to work with the image.
 *      c. reformat every single image in sub-picture directory.
*/
int main(int argc, char* argv[]) {

    QApplication app(argc,argv); // event loop


    // For test purposes please use multitudes of 16:9
    // provided pictures are in 16:9 format, otherwise program will crash,
    // I will implement a function which can convert all the pictures in the given folder to the appropriate
    // format, but for now, use the folowing settings:

    // Photo Mosaic Width: 15360
    // Photo Mosaic Height: 8640
    // Number of Sub Images: 14400
    // Sub Image Aspect Ratio: 16:9


    MainWindow mw;
    mw.show();
    PhotoMosaic pm;
    ImageInfo imgInfo;


    /* connecting signals with associated slots. */
    QObject::connect(&mw, SIGNAL(updateAll(QVector<QString>)),&imgInfo, SLOT(setAll(QVector<QString>)));
    QObject::connect(&imgInfo,SIGNAL(updateSubImageWidth(int)),&pm,SLOT(getSubImageWidth(int)));
    QObject::connect(&imgInfo,SIGNAL(updateSubImageHeight(int)),&pm,SLOT(getSubImageHeight(int)));
    QObject::connect(&imgInfo, SIGNAL(updateAll(QVector<QString>)),&pm,SLOT(runConcurrent(QVector<QString>)));



    app.exec();
    return 0;
}

