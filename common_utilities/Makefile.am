noinst_LTLIBRARIES = libCommonUtilities.la

simulator_dir = $(top_srcdir)/Simulator

libCommonUtilities_la_CPPFLAGS=@CLUSTERING_CPPFLAGS@
libCommonUtilities_la_CFLAGS=-DNEW_DIMEMAS_TRACE -DNANOSECOND_TRACE
libCommonUtilities_la_CXXFLAGS=-DNEW_DIMEMAS_TRACE -DNANOSECOND_TRACE

libCommonUtilities_la_SOURCES = \
	CommonMacros.h \
	Dimemas2Prv.c \
	Dimemas2Prv.h \
	Dimemas_Generation.c \
	Dimemas_Generation.h \
	Error.cpp \
	Error.hpp \
	EventEncoding.c \
	EventEncoding.h \
	ParaverColors.h

