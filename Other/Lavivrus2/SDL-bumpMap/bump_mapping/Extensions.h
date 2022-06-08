#ifndef GL_EXTENSIONS__INCLUDED
#define GL_EXTENSIONS__INCLUDED

#include <windows.h>
#include <gl/gl.h>

#define GL_TEXTURE_WRAP_R	0x8072

// GL_ARB_edge_clamp definitions

#define GL_CLAMP_TO_EDGE                  0x812F

// GL_ARB_texture_cube_map definitions

#define GL_NORMAL_MAP    0x8511
#define GL_REFLECTION_MAP    0x8512

#define GL_TEXTURE_CUBE_MAP    0x8513

#define GL_TEXTURE_BINDING_CUBE_MAP    0x8514

#define GL_TEXTURE_CUBE_MAP_POSITIVE_X     0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X     0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y     0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y     0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z     0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z     0x851A

#define GL_PROXY_TEXTURE_CUBE_MAP    0x851B

#define GL_MAX_CUBE_MAP_TEXTURE_SIZE    0x851C

// GL_ARB_texture_env_combine definitions

#define GL_COMBINE                                     0x8570

#define GL_COMBINE_RGB                                 0x8571
#define GL_COMBINE_ALPHA                               0x8572
#define GL_SOURCE0_RGB                                 0x8580
#define GL_SOURCE1_RGB                                 0x8581
#define GL_SOURCE2_RGB                                 0x8582
#define GL_SOURCE0_ALPHA                               0x8588
#define GL_SOURCE1_ALPHA                               0x8589
#define GL_SOURCE2_ALPHA                               0x858A
#define GL_OPERAND0_RGB                                0x8590
#define GL_OPERAND1_RGB                                0x8591
#define GL_OPERAND2_RGB                                0x8592
#define GL_OPERAND0_ALPHA                              0x8598
#define GL_OPERAND1_ALPHA                              0x8599
#define GL_OPERAND2_ALPHA                              0x859A
#define GL_RGB_SCALE                                   0x8573

#define GL_ADD_SIGNED                                  0x8574
#define GL_INTERPOLATE                                 0x8575
#define GL_SUBTRACT                                    0x84E7

#define GL_CONSTANT                                    0x8576
#define GL_PRIMARY_COLOR                               0x8577
#define GL_PREVIOUS                                    0x8578

// GL_ARB_texture_env_dot3 definitons

#define GL_DOT3_RGB                                    0x86AE
#define GL_DOT3_RGBA                                   0x86AF

// GL_ARB_multitexture definitions

#define GL_TEXTURE0			0x84C0
#define GL_TEXTURE1			0x84C1
#define GL_TEXTURE2			0x84C2
#define GL_TEXTURE3			0x84C3
#define GL_TEXTURE4			0x84C4
#define GL_TEXTURE5			0x84C5
#define GL_TEXTURE6			0x84C6
#define GL_TEXTURE7			0x84C7
#define GL_TEXTURE8			0x84C8
#define GL_TEXTURE9			0x84C9
#define GL_TEXTURE10		0x84CA
#define GL_TEXTURE11		0x84CB
#define GL_TEXTURE12		0x84CC
#define GL_TEXTURE13		0x84CD
#define GL_TEXTURE14		0x84CE
#define GL_TEXTURE15		0x84CF
#define GL_TEXTURE16		0x84D0
#define GL_TEXTURE17		0x84D1
#define GL_TEXTURE18		0x84D2
#define GL_TEXTURE19		0x84D3
#define GL_TEXTURE20		0x84D4
#define GL_TEXTURE21		0x84D5
#define GL_TEXTURE22		0x84D6
#define GL_TEXTURE23		0x84D7
#define GL_TEXTURE24		0x84D8
#define GL_TEXTURE25		0x84D9
#define GL_TEXTURE26		0x84DA
#define GL_TEXTURE27		0x84DB
#define GL_TEXTURE28		0x84DC
#define GL_TEXTURE29		0x84DD
#define GL_TEXTURE30		0x84DE
#define GL_TEXTURE31		0x84DF

#define GL_ACTIVE_TEXTURE                 0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE          0x84E1
#define GL_MAX_TEXTURE_UNITS              0x84E2

typedef void (APIENTRY * PFNGLACTIVETEXTUREPROC) (GLenum texture);
typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREPROC) (GLenum texture);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1DPROC) (GLenum target, GLdouble s);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1DVPROC) (GLenum target, const GLdouble *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1FPROC) (GLenum target, GLfloat s);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1FVPROC) (GLenum target, const GLfloat *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1IPROC) (GLenum target, GLint s);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1IVPROC) (GLenum target, const GLint *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1SPROC) (GLenum target, GLshort s);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1SVPROC) (GLenum target, const GLshort *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2DPROC) (GLenum target, GLdouble s, GLdouble t);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2DVPROC) (GLenum target, const GLdouble *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2FPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2FVPROC) (GLenum target, const GLfloat *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2IPROC) (GLenum target, GLint s, GLint t);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2IVPROC) (GLenum target, const GLint *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2SPROC) (GLenum target, GLshort s, GLshort t);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2SVPROC) (GLenum target, const GLshort *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3DPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3DVPROC) (GLenum target, const GLdouble *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3FPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3FVPROC) (GLenum target, const GLfloat *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3IPROC) (GLenum target, GLint s, GLint t, GLint r);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3IVPROC) (GLenum target, const GLint *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3SPROC) (GLenum target, GLshort s, GLshort t, GLshort r);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3SVPROC) (GLenum target, const GLshort *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4DPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4DVPROC) (GLenum target, const GLdouble *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4FPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4FVPROC) (GLenum target, const GLfloat *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4IPROC) (GLenum target, GLint s, GLint t, GLint r, GLint q);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4IVPROC) (GLenum target, const GLint *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4SPROC) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4SVPROC) (GLenum target, const GLshort *v);



extern PFNGLACTIVETEXTUREPROC			glActiveTexture;
extern PFNGLCLIENTACTIVETEXTUREPROC		glClientActiveTexture;
extern PFNGLMULTITEXCOORD1DPROC			glMultiTexCoord1d;
extern PFNGLMULTITEXCOORD1DVPROC			glMultiTexCoord1dv;
extern PFNGLMULTITEXCOORD1FPROC			glMultiTexCoord1f;
extern PFNGLMULTITEXCOORD1FVPROC			glMultiTexCoord1fv;
extern PFNGLMULTITEXCOORD1IPROC			glMultiTexCoord1i;
extern PFNGLMULTITEXCOORD1IVPROC			glMultiTexCoord1iv;
extern PFNGLMULTITEXCOORD1SPROC			glMultiTexCoord1s;
extern PFNGLMULTITEXCOORD1SVPROC			glMultiTexCoord1sv;
extern PFNGLMULTITEXCOORD2DPROC			glMultiTexCoord2d;
extern PFNGLMULTITEXCOORD2DVPROC			glMultiTexCoord2dv;
extern PFNGLMULTITEXCOORD2FPROC			glMultiTexCoord2f;
extern PFNGLMULTITEXCOORD2FVPROC			glMultiTexCoord2fv;
extern PFNGLMULTITEXCOORD2IPROC			glMultiTexCoord2i;
extern PFNGLMULTITEXCOORD2IVPROC			glMultiTexCoord2iv;
extern PFNGLMULTITEXCOORD2SPROC			glMultiTexCoord2s;
extern PFNGLMULTITEXCOORD2SVPROC			glMultiTexCoord2sv;
extern PFNGLMULTITEXCOORD3DPROC			glMultiTexCoord3d;
extern PFNGLMULTITEXCOORD3DVPROC			glMultiTexCoord3dv;
extern PFNGLMULTITEXCOORD3FPROC			glMultiTexCoord3f;
extern PFNGLMULTITEXCOORD3FVPROC			glMultiTexCoord3fv;
extern PFNGLMULTITEXCOORD3IPROC			glMultiTexCoord3i;
extern PFNGLMULTITEXCOORD3IVPROC			glMultiTexCoord3iv;
extern PFNGLMULTITEXCOORD3SPROC			glMultiTexCoord3s;
extern PFNGLMULTITEXCOORD3SVPROC			glMultiTexCoord3sv;
extern PFNGLMULTITEXCOORD4DPROC			glMultiTexCoord4d;
extern PFNGLMULTITEXCOORD4DVPROC			glMultiTexCoord4dv;
extern PFNGLMULTITEXCOORD4FPROC			glMultiTexCoord4f;
extern PFNGLMULTITEXCOORD4FVPROC			glMultiTexCoord4fv;
extern PFNGLMULTITEXCOORD4IPROC			glMultiTexCoord4i;
extern PFNGLMULTITEXCOORD4IVPROC			glMultiTexCoord4iv;
extern PFNGLMULTITEXCOORD4SPROC			glMultiTexCoord4s;
extern PFNGLMULTITEXCOORD4SVPROC			glMultiTexCoord4sv;

void SetUp_GL_ARB_multitexture();

#endif
