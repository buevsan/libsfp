
lib_LTLIBRARIES = libsfp.la
libsfp_la_SOURCES = libsfp.c libsfp_print.c 
libsfp_la_LDFLAGS = 

bin_PROGRAMS = sfp-dump
sfp_dump_SOURCES = sfp-dump.c
sfp_dump_LDFLAGS = -static 
sfp_dump_LDADD= ./libsfp.la

scriptsdir=$(bindir)
scripts_DATA=read-sfp-dump

x10includedir = $(includedir)
x10include_HEADERS = libsfp.h libsfp_regs.h libsfp_types.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libsfp.pc

DISTCLEANFILES = libsfp.pc
