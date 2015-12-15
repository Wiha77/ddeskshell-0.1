## src/Makefile.am

bin_PROGRAMS = ddeskshell

ddeskshell_SOURCES = ddeskshell.c ddeskshell.h 
ddeskshell_CFLAGS = $(LIBCONFIG_CFLAGS) $(GTK_CFLAGS) $(GLIB_CFLAGS) $(LIBNM_CFLAGS) $(LIBNMGLIB_CFLAGS) $(DBUSGLIB1_CFLAGS) $(LIBNMUTIL_CFLAGS) $(MMGLIB_CFLAGS)  -Dddesk_data_path='"$(pkgdatadir)"'
ddeskshell_LDADD = $(LIBCONFIG_LIBS) $(GTK_LIBS) $(GLIB_LIBS) $(LIBNM_LIBS) $(LIBNMGLIB_LIBS) $(DBUSGLIB1_LIBS) $(LIBNMUTIL_LIBS) $(MMGLIB_LIBS) 
 
