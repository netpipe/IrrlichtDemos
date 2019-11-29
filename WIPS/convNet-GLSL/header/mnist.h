
#ifndef MNIST_H
#define MNIST_H


class idxFileReader{
	
private:
	int numRows_train, numCols_train, numImages_train, numLabels_train;
	unsigned char** trainImages;
	unsigned char* trainLabels;
	int reverseInteger(int num);
	
public:
	idxFileReader(const char* filename_img, const char* filename_label);
	~idxFileReader();

	unsigned char* getImage(int imageID) { return trainImages[imageID]; }
	int getImageWidth() { return numCols_train; }
	int getImageHeight() { return numRows_train; }
	int getImageCount() { return numImages_train; }
	unsigned char getImageLabel(int imageID) { return trainLabels[imageID]; }

};


#endif
