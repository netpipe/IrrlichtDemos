#include "grabvideoframes.h"




// IF THE GIVEN FILE CONTAINS SPACE THEN IT WON'T RETURN THE REAL ADDRESS,
//WRITE A PROGRAM THAT READS A STRING AND IF IT CONTAINS SPACES, PUTS " RIGHT AFTER THE FIRST PREVIOUS \ AND PUTS ANOTHER " RIGHT BEFORE THE NEXT /

GrabVideoFrames::GrabVideoFrames(int imageWidth, int imageHeight, QString durationFile)
{
    m_videoPath = QFileDialog::getOpenFileName(0,"Select the Video ","/home",
                           "Videos ( *.mkv *.avi *.mov *.mp4 ))");


    m_destinationFolder = QFileDialog::getExistingDirectory(0, "Select the destination folder for extracted pictures",
                                       "/home",
                                       QFileDialog::ShowDirsOnly
                                       | QFileDialog::DontResolveSymlinks);

    qDebug() << m_videoPath;
    qDebug() << m_destinationFolder;
    m_imageWidth = imageWidth;
    m_imageHeight = imageHeight;
    m_duration = getDurationInSeconds(durationFile);
    qDebug() << "DURATION: " << m_duration;

}

GrabVideoFrames::GrabVideoFrames(int imageWidth, int imageHeight, QString videoPath, QString destinationFolder, QString durationFile)
{
    m_videoPath = videoPath;


    m_destinationFolder = destinationFolder;

    qDebug() << m_videoPath;
    qDebug() << m_destinationFolder;
    m_imageWidth = imageWidth;
    m_imageHeight = imageHeight;
    m_duration = getDurationInSeconds(durationFile);
    qDebug() << "DURATION: " << m_duration;

}


//given frequency (seconds/ per frame) and numOfPictures to be extracted count and its pattern,
//extract the frames from the beggining of the file until the end.
//startingSec determines where to start
//and startNumber determines the starting name of the frame. (by default its 0 so 0.jpeg, 1.jpeg, etc)
void GrabVideoFrames::extractPictures(double frequency, int numOfPictures, int startingSec, int startNumber)
{
    QElapsedTimer timer;
    timer.start();
    QProcess process;

    QString extractPictureCommand;

    qDebug() << numOfPictures;
    // if numOfPictures is larger than the duration the only extract enough pictures for duration.
    if (numOfPictures  > m_duration) {
    numOfPictures = m_duration;
    }
    qDebug() << "NUM OF PIC: " << numOfPictures;
    qDebug() << m_duration;
    frequency = 1 / frequency; // ffmpeg represents -r in per frame form so in order to get 1 frame 2 second, 1/2second = 0.5 ==> 0.5 frame per second


    // even though default value for start_number is 0, ffmpeg won't accept 0, in order to call the default value start_number shouldn't be called.
    //remove this
    if (startNumber == 0) {
    extractPictureCommand = QString("ffmpeg -ss %1 -threads 16  -loglevel 0 -i %2 -r %3 -t %4  -s %5x%6 %7/%d.jpeg")
            .arg(startingSec)
            .arg(m_videoPath)
            .arg(QString::number(frequency))
            .arg(QString::number(numOfPictures))
            .arg(m_imageWidth)
            .arg(m_imageHeight)
            .arg(m_destinationFolder);
    } else {
    extractPictureCommand = QString("ffmpeg -ss %1 -threads 16  -loglevel 0 -i %2 -r %3 -t %4  -start_number %5 -s %6x%7 %8/%d.jpeg")
        .arg(startingSec)
        .arg(m_videoPath)
        .arg(QString::number(frequency))
        .arg(QString::number(numOfPictures))
        .arg(QString::number(startNumber))
        .arg(m_imageWidth)
        .arg(m_imageHeight)
        .arg(m_destinationFolder);
    }

    qDebug() << extractPictureCommand;
    process.execute(extractPictureCommand);
    qDebug() << "Took" << timer.elapsed() << "milliseconds";


    m_count =  startNumber + (numOfPictures * frequency);


}

int GrabVideoFrames::getCount() const
{
    return m_count;
}

int GrabVideoFrames::getDuration() const
{
    return m_duration;
}

// extract pictures from a movie at random time which never exceeds the duration of the movie,
// this function is useless when large number of pictures needed from a video file.
// if 50% of frames needed from a picture it's faster and more accurate to extract all frames then select randomly
// among them. that way no repetition is ensured and it's faster.
//by default, count starts from 0 and count pattern is 1 (count++).
void GrabVideoFrames::extractPicturesRANDOM(int numOfPictures, int count, int countPattern)
{
    qDebug() << "Starting to Extract Pictures";

    QElapsedTimer timer;
    timer.start();

    QProcess process;

    int randFrameAt = qrand() % ((m_duration + 1) - 1) + 1; //frame to be captured, randomly selected.



    //if  -ss position applied before -i it will be VERY FAST but less accurate
    //see http://ffmpeg.org/trac/ffmpeg/wiki/Seeking%20with%20FFmpeg
    //loglevel -0 supresses the output.

    for (int i = 0; i < numOfPictures; i++) {

    QString extractPictureCommand = QString("ffmpeg  -loglevel 0 -ss %1 -i %2 -r 1 -t 1 -s %3x%4 -f image2 %5/%6.jpeg")
        .arg(randFrameAt)
        .arg(m_videoPath)
        .arg(m_imageWidth)
        .arg(m_imageHeight)
        .arg(m_destinationFolder)
        .arg(QString::number(count));

    process.execute(extractPictureCommand);
    randFrameAt = qrand() % ((m_duration + 1) - 1) + 1;
    count += countPattern;

    }
    qDebug() << "Extracted Pictures!";
    qDebug() << "Took" << timer.elapsed() << "milliseconds";

}


//heavily depends on how ffmpeg display the information to the screen,
//usually Duration hh:mm:ss, so find Duration then take everything up to first , which will give "hh:mm:ss" convert that into seconds
// return the seconds.
int GrabVideoFrames::getDurationInSeconds(QString durationFile) const
{
    QString durationCommand = QString("ffmpeg -i  %1 2> %2")
        .arg(m_videoPath)
        .arg(durationFile);

    char *charCommand = new char[durationCommand.size() + 1];
    for(int i=0; i<durationCommand.size(); i++) {
    charCommand[i] = durationCommand.at(i).toAscii();
    }
    system(charCommand);
    delete[] charCommand;

    QFile file(durationFile);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString line = in.readAll();
    line = line.section("Duration: ",1,1);
    line = line.section(",",0,0);
    line = line.section(".",0,0);
    qDebug() << line;
    return convertToSeconds(line);
}

//convert hh:mm:ss to seconds.
int GrabVideoFrames::convertToSeconds(QString hhmmss) const
{
    QStringList strList = hhmmss.split(":");
    int hours = strList.value(0).toInt();
    int min = strList.value(1).toInt();
    int sec = strList.value(2).toInt();
    int res = (hours * 60 * 60) + (min * 60) + sec;
    return res;
}

