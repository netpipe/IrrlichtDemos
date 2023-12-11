AC_INIT([src/Azadi.cpp])

PACKAGE=azadi
VERSION=0.1

dnl Init the automake system.
AM_INIT_AUTOMAKE($PACKAGE, $VERSION)

dnl We need a C, a C++ compiler, and an install script
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

AC_PATH_PROGS(BASH, bash sh)

AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stddef.h stdlib.h string.h sys/socket.h])
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_STRUCT_TM
AC_FUNC_REALLOC
AC_CHECK_FUNCS([atexit memmove memset sqrt strchr])

AC_ARG_ENABLE(debug,
	[AS_HELP_STRING([--disable-debug], [Do not optimize (default is yes currently)])],
	[CFLAGS="-O2 -DNDEBUG"], [CFLAGS="-g -D_DEBUG"])
CXXFLAGS=$CFLAGS

dnl AM_CONDITIONAL(DEBUG, test "$enable_debug" = yes)

AC_CHECK_LIB(GL, glLoadIdentity,,
		 AC_MSG_ERROR([You need to install OpenGL. Visit http://www.opengl.org]))
AC_CHECK_LIB(GLU, gluLookAt,,
		 AC_MSG_ERROR([You need libGLU.]))
 AC_CHECK_LIB(GLEW, glewInit,,
		 AC_MSG_ERROR([You need libGLEW.]))
AC_CHECK_LIB(png, png_read_image,,
		 AC_MSG_ERROR([You need libpng.]))
AC_CHECK_LIB(jpg, jpeg_read_header,
		 AC_MSG_ERROR([You need libjpeg.]))

dnl SupraSocket
AC_CHECK_LIB(SupraSocket, main,,
		 AC_MSG_ERROR([You need to install libSupraSocket. Visit: http://www.yomogi-soft.com/suprasocket]))

dnl libstdc++
AC_CHECK_LIB(stdc++, main,, AC_MSG_ERROR(libstdc++ is required.))

dnl Check for some modules
PKG_CHECK_MODULES(sdl, "sdl")
PKG_CHECK_MODULES(openal, "openal")
PKG_CHECK_MODULES(ogg, "ogg")
PKG_CHECK_MODULES(vorbis, "vorbis")
PKG_CHECK_MODULES(vorbisfile, "vorbisfile")
PKG_CHECK_MODULES(lua, "lua")
PKG_CHECK_MODULES(theora, "theora")
dnl STore their CFLAGS and LIBS
AC_SUBST(sdl_CFLAGS)
AC_SUBST(sdl_LIBS)
AC_SUBST(openal_CFLAGS)
AC_SUBST(openal_LIBS)
AC_SUBST(ogg_CFLAGS)
AC_SUBST(ogg_LIBS)
AC_SUBST(vorbis_CFLAGS)
AC_SUBST(vorbis_LIBS)
AC_SUBST(vorbis_CFLAGS)
AC_SUBST(vorbis_LIBS)
AC_SUBST(lua_CFLAGS)
AC_SUBST(lua_LIBS)
AC_SUBST(theora_CFLAGS)
AC_SUBST(theora_LIBS)

dnl AC_CHECK_PROG (variable, prog-to-check-for, value-if-found, [value-if-not-found], [path], [reject])
AC_CHECK_PROG(FTCONF, [freetype-config], [freetype-config], [no])
if test "x$FTCONF" = "xno"; then
   AC_MSG_ERROR([freetype-config not found. Please install freetype])
fi
freetype_CFLAGS=`freetype-config --cflags`
freetype_LIBS=`freetype-config --libs`
AC_SUBST(freetype_CFLAGS)
AC_SUBST(freetype_LIBS)

AC_OUTPUT([Makefile src/Makefile])
