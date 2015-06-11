
lib_LTLIBRARIES = libx10-sfp.la
libx10_sfp_la_SOURCES = libsfp.c
libx10_sfp_la_LDFLAGS = -static

bin_PROGRAMS = sh-sfp
sh_sfp_LDADD=./libx10-sfp.la

x10includedir = $(includedir)/x10
x10include_HEADERS = libsfp.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = x10-sfp.pc

DISTCLEANFILES = x10-sfp.pc
