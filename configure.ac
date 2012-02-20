#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.61)
AC_INIT(FULL-PACKAGE-NAME, VERSION, BUG-REPORT-ADDRESS)
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
AC_CHECK_HEADERS([string.h Xlib.h malloc/malloc.h])

# Checks for typedefs, structures, and compiler characteristics.
PKG_CHECK_MODULES([X11], x11)

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
