AC_PREREQ([2.63])
AC_INIT([libsfp],
        [0.0.6],
        [http://bugzilla.metrotek.spb.ru/enter_bug.cgi?product=libsfp],
        [libsfp])

AC_CONFIG_MACRO_DIR([m4])

AM_PROG_LIBTOOL
AC_ARG_ENABLE([rpath],
    [AS_HELP_STRING([--disable-rpath],
        [do not set 'rpath' in the libraries (default is auto)])],
    hardcode_into_libs=$enableval)

AM_INIT_AUTOMAKE([1.11 -Wno-portability dist-xz no-dist-gzip])
AM_MAINTAINER_MODE([enable])

AC_SUBST(LIBSFP_CFLAGS)
AC_SUBST(LIBSFP_LIBS)

INSTALL="$INSTALL -p"  # preserve the timestamps

AC_OUTPUT([Makefile libsfp.pc])
