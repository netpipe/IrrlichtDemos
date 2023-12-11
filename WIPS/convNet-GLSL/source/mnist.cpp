
#include "../header/mnist.h"
#include <fstream>
#include <iostream>
using namespace std;

idxFileReader::idxFileReader(const char* filename_img, const char* filename_label)
{
	int magic_number, n_rows, n_cols, number_of_images, image_size;
	
	std::ifstream trainSet(filename_img, ios::binary);
	if(trainSet.is_open())
	{
		trainSet.read((char*)&magic_number, sizeof(magic_number));
		magic_number = reverseInteger(magic_number);
		if(magic_number != 2051) std::cout<<std::endl<<" Invalid File::File Type "<<magic_number<<std::endl;
		else
		{
			trainSet.read((char *)&number_of_images, sizeof(number_of_images));
			 	number_of_images = reverseInteger(number_of_images);
    	    trainSet.read((char *)&n_rows, sizeof(n_rows));
    	    	n_rows = reverseInteger(n_rows);
    	    trainSet.read((char *)&n_cols, sizeof(n_cols));
    	    	n_cols = reverseInteger(n_cols);
		
			idxFileReader::numRows_train = n_rows;
			idxFileReader::numCols_train = n_cols;
			idxFileReader::numImages_train = number_of_images;
			image_size = n_rows * n_cols;
		
			trainImages = new unsigned char* [number_of_images];
        	for(int i = 0; i < number_of_images; i++) 
        	{
            	trainImages[i] = new unsigned char[image_size];
            	trainSet.read((char *)trainImages[i], image_size);
            }
		
		}
	
	}
	else
	{
		std::cout<<std::endl<<" could not open training Image Set "<<std::endl;
		
	}
	
	
	//now for labels of Images
	std::ifstream trainSet_label(filename_label, ios::binary);
	if(trainSet_label.is_open())
	{
		trainSet_label.read((char*)&magic_number, sizeof(magic_number));
		magic_number = reverseInteger(magic_number);
		if(magic_number != 2049) std::cout<<std::endl<<" Invalid Label File::File Type "<<magic_number<<std::endl;
		else
		{
			trainSet_label.read((char *)&numLabels_train, sizeof(numLabels_train));
			 	numLabels_train = reverseInteger(numLabels_train);
		
			trainLabels = new unsigned char [numLabels_train];
        	for(int i = 0; i < numLabels_train; i++) 
            	trainSet_label.read((char *)&(trainLabels[i]), 1);
            
		}
	
	}
	else
	{
		std::cout<<std::endl<<" could not open training label Set "<<std::endl;
		
	}

	
	
}


idxFileReader::~idxFileReader()
{
	delete[] trainImages;
	delete trainLabels;
}

int idxFileReader::reverseInteger(int num)
{
	unsigned char c1, c2, c3, c4;
	c1 = num & 255;
	c2 = (num>>8) & 255;
	c3 = (num>>16) & 255;
	c4 = (num>>24) & 255;
			
	return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}




