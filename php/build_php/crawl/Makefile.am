AUTOMAKE_OPTIONS = foreign no-dependencies

bin_PROGRAMS = crawl
crawl_SOURCES= crawl.c http.c connection.c atomicio.c html.c crawldb.c util.c \
	dns.c cfg.c robots.c \
	connection.h dns.h http.h tree.h cfg.h crawldb.h html.h \
	robots.h util.h

crawl_LDADD= @LIBOBJS@ @EVENTLIB@ @DBLIB@

CFLAGS += -Wall
INCLUDES = -I$(top_srcdir)/compat @EVENTINC@ @DBINC@

man_MANS = crawl.1

EXTRA_DIST = crawl.conf acconfig.h $(man_MANS) \
	basename.c dirname.c err.c getaddrinfo.c inet_aton.c md5.c \
	strlcat.c strlcpy.c strsep.c \
	compat/err.h compat/gai-errnos.h compat/getaddrinfo.h compat/md5.h \
	compat/sys/queue.h

DISTCLEANFILES = *~
