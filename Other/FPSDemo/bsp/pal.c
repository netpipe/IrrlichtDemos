#include "engine.h"

byte   RGBPalette[768];
byte *PalTables[32];
int PalNumTables,PalNumShades,PalNumShades2;
static byte *AllocPalTable;


int InitPalette(char *name)
{
	FILE *fp;
	byte temp[2], *ptr;
	int size,i;

	fp=fopen(name,"rb");
	if (fp==NULL) {
	    printf("Failed to load palette file");
   //!     MessageBox(hWnd,"Failed to load palette file.","Error",MB_OK|MB_ICONERROR);
		return 0;
    }
// Read actual palette
	fread(RGBPalette,1,768,fp);

// Read NumTables and NumShades2
	if (fread(temp,1,2,fp)<2) {
		PalNumTables=0;
		PalNumShades=0;
		PalNumShades2=0;
		fclose(fp);
     //!   MessageBox(hWnd,"Failed to read palette.","Error",MB_OK|MB_ICONERROR);
		return 0;
	}
	PalNumTables=temp[0];
	PalNumShades2=temp[1];
	PalNumShades=1<<PalNumShades2;
	size=(256<<PalNumShades2)*PalNumTables;
// Allocate memory for tables
// For now it has to be allocated at 256 boundary
	AllocPalTable=(unsigned char*)malloc(size+256);
	ptr=(BYTE *)(((unsigned long)AllocPalTable+255)&0xFFFFFF00);
// Load tables
	fread(ptr,1,size,fp);
	fclose(fp);

// Make pointers
	for(i=0;i<PalNumTables;i++) {
		PalTables[i]=ptr;
		ptr+=256<<PalNumShades2;
	}
	return 1;
}



void FreePalette(void)
{
	if (AllocPalTable) {
		free(AllocPalTable);
		AllocPalTable=PalTables[0]=NULL;
	}
}
