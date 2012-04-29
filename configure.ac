#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.61)
AC_INIT([Nounours], 1.0, BUG-REPORT-ADDRESS)
AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_HEADER([config/config.h])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O

# Checks for libraries.



# Checks for header files.
AC_PATH_X
AC_HEADER_STDC
AC_CHECK_HEADERS([string.h Xlib.h jpeglib.h pthread.h unistd.h math.h])



# Checks for typedefs, structures, and compiler characteristics.
 	    
 	    
PKG_CHECK_MODULES([X11], x11)
AC_ARG_WITH([jpeg-include-path],
  [AS_HELP_STRING([--with-jpeg-include-path],
    [location of the jpeglib.h, defaults to /usr/include/])],
  [JPEG_CFLAGS="-I$withval"],
  [JPEG_CFLAGS='-I/usr/include/'])
AC_SUBST([JPEG_CFLAGS])

AC_ARG_WITH([jpeg-lib-path],
  [AS_HELP_STRING([--with-jpeg-lib-path], [location of the libjpeg library])],
  [JPEG_LIBS="-L$withval -ljpeg"],
  [JPEG_LIBS='-ljpeg'])
AC_SUBST([JPEG_LIBS])

AC_ARG_WITH([xscreensaver-config-path],
  [AS_HELP_STRING([--with-xscreensaver-config-path],
    [location of the xscreensaver hack xmls files, defaults to /usr/share/xscreensaver/config/])],
  [XSCREENSAVER_CONFIG_PATH="$withval"],
  [XSCREENSAVER_CONFIG_PATH='/usr/share/xscreensaver/config'])
AC_SUBST([XSCREENSAVER_CONFIG_PATH])

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
