#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ddeskshell], [0.1], [wiha77@gmail.com])
AC_CONFIG_SRCDIR([src/ddeskshell.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE

#AC_DEFINE_UNQUOTED(ddesk_data_path,${prefix},Path data dir)

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([locale.h stdlib.h string.h libconfig.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
PKG_CHECK_MODULES([LIBCONFIG], [libconfig >= 1.4],,
  AC_MSG_ERROR([libconfig 1.4 or newer not found.])
)
PKG_CHECK_MODULES([GTK], [gtk+-3.0])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([LIBNM], [NetworkManager])
PKG_CHECK_MODULES([LIBNMGLIB], [libnm-glib])
PKG_CHECK_MODULES([DBUSGLIB1], [dbus-glib-1])
PKG_CHECK_MODULES([LIBNMUTIL], [libnm-util])
PKG_CHECK_MODULES([MMGLIB], [mm-glib])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 data/Makefile])
AC_OUTPUT
