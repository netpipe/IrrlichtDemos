#include <stdio.h>
#include <malloc.h>
#include <memory.h>

#pragma pack (push, 1)


typedef struct 			// PCX file header
{
	byte manufacturer;   // 0x0a --signifies a PCX file
	byte version;        // version 5 is what we look for
	byte encoding;       // when 1,it's RLE encoding (only type as of yet)
	byte bitsperpixel;   // how many bits to represent 1 pixel
	word xmin, ymin, xmax, ymax; // dimensions of window
	word HDPI, VDPI;     // device resolution (horizontal,vertical)
	byte colormap[16*3]; // 16-color palette
	byte reserved;
	byte nplanes;        // number of color planes
	word bytesperline;   // number of bytes per line (per color plane)
	word palette_info;   // 1 = color,2 = grayscale (unused in v.5+)
	byte filler[58];     // used to fill-out 128 byte header (useless)
} PcxHeader;


byte *PCX_Decode(byte *pcx, int *width, int *height, byte *pal)
{
	PcxHeader *header;
	byte *src, *dest, data;
	int len, temp, offset, count, si, di, h, w;


// Get header
	header=(PcxHeader *)pcx;
	src=pcx+sizeof(PcxHeader);


// If not a PCX file, or unknown type, return invalid
	if (header->manufacturer!=0x0a||
	    header->version!=5||
	    header->encoding!=1) return 0;

// If it's not a 256-color PCX file, return invalid
	if (header->nplanes!=1||header->bitsperpixel!=8)
		return 0;

// Calculate the width and height of image (see notes above on width)
// Bytes-per-line is used, since it includes padding count
	*width=w=header->bytesperline;
	*height=h=header->ymax-header->ymin+1;

	len=w*h;

// Now allocate space to save the image in
	dest=(byte *)calloc(len,1);
	if (!dest) return 0;

// Going row by row, read the data from the src to raw
	si=0;			// indexes for src
	temp=h;

	while (temp--) {
		offset=0;	// init offset count
		di=(h-temp-1)*w;
		while(offset<w) {
			data=src[si++];	// get byte of compressed data
			if ((data&0xc0)==0xc0) { // is it count byte?
				count=data&0x3f; // mask off repeat count
				data=src[si++];	 // get data to repeat

				offset+=count;	 // increase offset
				while(count--) {
					if (di>len) {	// wrong di
						free(src);
						free(dest);
						return NULL;
					}
					dest[di]=data; di++;
				}
			}
			else {		// just a normal data byte
				if (di>len) { // wrong di
					free(src);
					free(dest);
					return NULL;
				}
				dest[di]=data;
				di++;
				offset++;
			}
		}
	}

// Read in the palette
	if (src[si++]==0xc&&pal) {
		memcpy(pal,src+si,768);
	}


	return(dest);
}



typedef struct {	// TGA file header
	byte id_length, colormap_type, image_type;
	word colormap_index, colormap_length;
	byte colormap_size;
	word x_origin, y_origin, width, height;
	byte pixel_size, attributes;
} TgaHeader;



byte *TGA_Decode(byte *tga, int *width, int *height)
{
	int columns, rows, numPixels;
	int row, column;
	byte *rgba, *dest;
	TgaHeader *header;
	byte red,green,blue,alpha,packetHeader,packetSize,j;

	header = (TgaHeader *)tga;
	tga+=sizeof(TgaHeader);


// Only types 2 and 10 are supported
	if (header->image_type!=2 && header->image_type!=10) {
		return 0;
	}

// Only 24bit or 32bit pixels are supported
	if (header->colormap_type!=0||
		(header->pixel_size!=32&&header->pixel_size!=24)) {
		return 0;
	}

	columns = header->width;
	rows = header->height;
	numPixels = columns * rows;

	if (width) *width = columns;
	if (height) *height = rows;

// Allocate memory for decoded image
	rgba = (unsigned char*)malloc(numPixels*4);
	if (!rgba) {
		return 0;
	}

// Skip comment
	if (header->id_length!=0)
		tga+=header->id_length;
	
// Uncompressed RGB image
	if (header->image_type==2) {
		for(row=rows-1; row>=0; row--) {
			dest = rgba + row*columns*4;
			for(column=0; column<columns; column++) {
				if (header->pixel_size==24) {
					blue = *tga; tga++;
					green = *tga; tga++;
					red = *tga; tga++;
					*dest++ = red;
					*dest++ = green;
					*dest++ = blue;
					*dest++ = 255;
				}
				else if (header->pixel_size==32) {
					blue = *tga; tga++;
					green = *tga; tga++;
					red = *tga; tga++;
					alpha = *tga; tga++;
					*dest++ = red;
					*dest++ = green;
					*dest++ = blue;
					*dest++ = alpha;
				}
			}
		}
	}
// RLE RGB image
	else if (header->image_type==10) {
		for(row=rows-1; row>=0; row--) {
			dest = rgba + row*columns*4;
			for(column=0; column<columns; ) {
				packetHeader=*tga; tga++;
				packetSize = (unsigned char)(1 + (packetHeader & 0x7f));
			// RLE packet
				if (packetHeader & 0x80) {
					if (header->pixel_size==24) {
						blue = *tga; tga++;
						green = *tga; tga++;
						red = *tga; tga++;
						alpha = 255;
					}
					else if (header->pixel_size==32) {
						blue = *tga; tga++;
						green = *tga; tga++;
						red = *tga; tga++;
						alpha = *tga; tga++;
					}
	
					for(j=0;j<packetSize;j++) {
						*dest++=red;
						*dest++=green;
						*dest++=blue;
						*dest++=alpha;
						column++;
						if (column==columns) {
							column=0;
							if (row>0)
								row--;
							else
								goto end_decode;
							dest = rgba + row*columns*4;
						}
					}
				}
			// Non-RLE packet
				else {
					for(j=0;j<packetSize;j++) {
						if (header->pixel_size==24) {
							blue = *tga; tga++;
							green = *tga; tga++;
							red = *tga; tga++;
							*dest++ = red;
							*dest++ = green;
							*dest++ = blue;
							*dest++ = 255;
						}
						else {
							blue = *tga; tga++;
							green = *tga; tga++;
							red = *tga; tga++;
							alpha = *tga; tga++;
							*dest++ = red;
							*dest++ = green;
							*dest++ = blue;
							*dest++ = alpha;
						}
						column++;
						if (column==columns) {
							column=0;
							if (row>0)
								row--;
							else
								goto end_decode;
							dest = rgba + row*columns*4;
						}						
					}
				}
			}
			end_decode:;
		}
	}	

	return(rgba);
}

