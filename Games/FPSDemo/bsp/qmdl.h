
/*
========================================================================

.MD2 triangle model file format

========================================================================
*/

#define IDALIASHEADER	(('2'<<24)+('P'<<16)+('D'<<8)+'I')
#define ALIAS_VERSION	8

#define	MAX_TRIANGLES	4096
#define MAX_VERTS	2048
#define MAX_FRAMES	512
#define MAX_MD2SKINS	32
#define	MAX_SKINNAME	64

typedef struct
{
	short	s;
	short	t;
} dstvert_t;

typedef struct 
{
	short	index_xyz[3];
	short	index_st[3];
} dtriangle_t;

typedef struct
{
	byte	v[3];		// scaled byte to fit in frame mins/maxs
	byte	lightnormalindex;
} dtrivertx_t;

#define DTRIVERTX_V0   0
#define DTRIVERTX_V1   1
#define DTRIVERTX_V2   2
#define DTRIVERTX_LNI  3
#define DTRIVERTX_SIZE 4

typedef struct
{
	float		scale[3];	// multiply byte verts by this
	float		translate[3];	// then add this
	char		name[16];	// frame name from grabbing
	dtrivertx_t	verts[1];	// variable sized
} daliasframe_t;


// the glcmd format:
// a positive integer starts a tristrip command, followed by that many
// vertex structures.
// a negative integer starts a trifan command, followed by -x vertexes
// a zero indicates the end of the command list.
// a vertex consists of a floating point s, a floating point t,
// and an integer vertex index.


typedef struct
{
	int	ident;
	int	version;

	int	skinwidth;
	int	skinheight;
	int	framesize;		// byte size of each frame

	int	num_skins;
	int	num_xyz;
	int	num_st;			// greater than num_xyz for seams
	int	num_tris;
	int	num_glcmds;		// dwords in strip/fan command list
	int	num_frames;

	int	ofs_skins;		// each skin is a MAX_SKINNAME string
	int	ofs_st;			// byte offset from start for stverts
	int	ofs_tris;		// offset for dtriangles
	int	ofs_frames;		// offset for first frame
	int	ofs_glcmds;	
	int	ofs_end;		// end of file
} dmdl_t;

