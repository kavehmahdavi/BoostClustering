## Process this file with automake to produce Makefile.in
## Created by Anjuta

AM_CPPFLAGS =\
 $(AM_CPPFLAGS) \
 -I$(top_srcdir)/include\
 -I$(top_srcdir)/common_utilities

##    -I@common_filesdir@

## The 'pcfparser_svn3942' and 'common_utilities' directory will
## be merged in the new 'common-files' package
SUBDIRS = include pcfparser_svn3942 common_utilities src scripts doc

instdir = $(includedir)

inst_HEADERS = \
	clustering_suite_config.h

EXTRA_DIST = \
  $(top_srcdir)/example \
  $(top_srcdir)/include \
  $(top_srcdir)/substitute \
  $(top_srcdir)/substitute-all

# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(clusteringsuitedocdir)

# Install the example
EXAMPLE_FILES = \
 example/cluster_explained.xml\
 example/cluster.xml\
 example/gromacs_64p.chop1.pcf\
 example/gromacs_64p.chop1.prv\
 example/gromacs_64p.chop1.row

DOC_FILES = \
 doc/clusteringsuite_manual.pdf
 
install-data-hook: $(EXAMPLE_FILES) $(DOC_FILES)
	$(MKDIR_P) $(DESTDIR)$(datadir)/example
	cp -f $(EXAMPLE_FILES) $(DESTDIR)$(datadir)/example
	$(MKDIR_P) $(DESTDIR)$(datadir)/doc
	cp -f $(DOC_FILES) $(DESTDIR)$(datadir)/doc
