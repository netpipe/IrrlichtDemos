#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define IDPAKHEADER	(('K'<<24)+('C'<<16)+('A'<<8)+'P')
#define	MAX_FILES_IN_PACK	4096

typedef struct
{
	char	name[56];
	int	filepos, filelen;
} dpackfile_t;

typedef struct
{
	int	ident;		// == IDPAKHEADER
	int	dirofs;
	int	dirlen;
} dpackheader_t;

static dpackheader_t PakHeader;
static dpackfile_t PakFiles[MAX_FILES_IN_PACK];
static int NumPakFiles=0;
static FILE *pakFile;
extern char pakPath[64];


int DAT_Open(char *fpath)
{
	FILE *fp;
	char fname[64];

	fp=fopen(fpath,"rb");
	if (!fp)
	{
//        	MessageBox(hWnd,"Failed to load Pak file.","Error",MB_OK|MB_ICONERROR);
            	return 0;
	}
	fread(&PakHeader,sizeof(dpackheader_t),1,fp);
	if (PakHeader.ident!=IDPAKHEADER) {
		fclose(fp);
   //             MessageBox(hWnd,"Pak header ID wrong.","Error",MB_OK|MB_ICONERROR);
		return 0;
	}

	fseek(fp,PakHeader.dirofs,SEEK_SET);
	fread(&PakFiles,1,PakHeader.dirlen,fp);

	NumPakFiles=PakHeader.dirlen/sizeof(dpackfile_t);

	pakFile=fp;

	return 1;
}


void DAT_Close()
{
	NumPakFiles=0;
	if (pakFile) {
		fclose(pakFile);
		pakFile=0;
	}
}


void *DAT_ReadFile(char *name,int *psize)
{
	FILE *fp;
	char fname[64], *ptr;
	byte *raw=0;
	int size, i;

// Make full path name to the file in pakPath directory
	strcpy(fname,pakPath);
	strcat(fname,name);
// Convert '/' into '\'
	ptr=fname;
	while(*ptr) {
		if ((*ptr)=='/') *ptr='\\';
		ptr++;
	}

// Try open file in current directory
	fp=fopen(fname+strlen(pakPath),"rb");

// Nope... try opening file in pakPath directory
	if (!fp) fp=fopen(fname,"rb");

// Got normal file
	if (fp) {
		fseek(fp,0,SEEK_END);
		size=ftell(fp);
		fseek(fp,0,SEEK_SET);
		raw=(unsigned char*)malloc(size);
		if (raw) {
			fread(raw,1,size,fp);
			*psize=size;
		}
		fclose(fp);
	}
// Get it from resource file
	else {
		for(i=0;i<NumPakFiles;i++) {
			//if (stricmp(PakFiles[i].name,name)==0) {
            if (strcasecmp(PakFiles[i].name,name)==0) {
				size=PakFiles[i].filelen;
				raw=(unsigned char*)malloc(size);
				if (raw) {
					fseek(pakFile,
						PakFiles[i].filepos,
						SEEK_SET);
					fread(raw,1,size,pakFile);
					*psize=size;
				}
				break;
			}
		}
	}
	return raw;
}
