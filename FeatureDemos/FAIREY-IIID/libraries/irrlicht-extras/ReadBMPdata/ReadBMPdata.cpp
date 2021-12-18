/* Reads BMP images data */
/* In Windows Photoshop, BMP is saved this way: - File format: Windows OS - Depth: 24 Bits -  Flip row order = True */

int BMP_width  = 64;
int BMP_height = 32;
int BMP_data_ordered[64*32][3];


/// Load BMP image file:
void readFileBITMAP(char* filename) {

    FILE* file = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, file); // Read the 54-byte header

    /// Extract image height and width from header
    /// BMP_width  = *(int*)&info[18];
    /// BMP_height = *(int*)&info[22];
    int heightSign = 1;
    if(BMP_height<0){ heightSign = -1; };

    int bsize = 3*BMP_width*abs(BMP_height);
    unsigned char* BMP_data = new unsigned char[bsize]; // Allocate 3 bytes per pixel
    fread(BMP_data, sizeof(unsigned char), bsize, file); // Read the rest of the data at once
    fclose(file);

    int row = 0;
    for(int i=0; i<bsize; i+=3) {
    BMP_data_ordered[row][0] = BMP_data[i+0];
    BMP_data_ordered[row][1] = BMP_data[i+1];
    BMP_data_ordered[row][2] = BMP_data[i+2];
    row ++;
    };

    delete [] BMP_data;
};


/*
Now BMPdata should contain the (R,G,B) values of the pixels.
The color of pixel(i,j) is stored at BMPdata[3*(i*BMPwidth+j)], BMPdata[3*(i*BMPwidth+j)+1] and BMPdata[3*(i*BMPwidth+j)+2].

In the last part, the swap between every first and third pixel is done because windows
stores the color values as (B, G, R) triples, not (R, G, B).
*/
