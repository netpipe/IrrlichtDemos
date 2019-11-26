#ifndef GRABVIDEOFRAMES_H
#define GRABVIDEOFRAMES_H

#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QElapsedTimer>
#include <QProcess>

/* This class meant to give you an idea of how to capture frames using ffmpeg, (without having to worry about the API) the actual photomosaic software doesn't use this class.
 * Originally I was using FFMPEG API to capture the images programatically but this approach seemed bit easier.
 * You need to install ffmpeg and add it to your PATH to be able to use any of the functions defined in this class.
*/
class GrabVideoFrames
{
public:
    GrabVideoFrames(int imageWidth, int imageHeight, QString durationFile = "Duration.txt");
    GrabVideoFrames(int imageWidth, int imageHeight, QString videoPath, QString destinationFolder, QString durationFile = "Duration.txt");

    int getCount() const;
    int getDuration() const;

    void extractPictures(double frequency, int numOfPictures, int startingSec = 0, int startNumber = 0);
    void extractPicturesRANDOM(int numOfPictures, int count = 0, int countPattern = 1);

private:
    QString m_videoPath;
    QString m_destinationFolder;
    int m_imageWidth;
    int m_imageHeight;
    int m_duration;
    int m_count;

    int getDurationInSeconds(QString durationFile) const;
    int convertToSeconds(QString hhmmss) const;

};

#endif // GRABVIDEOFRAMES_H
