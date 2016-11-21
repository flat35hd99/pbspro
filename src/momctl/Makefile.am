


sbin_PROGRAMS = momctl

momctl_CPPFLAGS = \
	-I$(top_srcdir)/src/include \
	@KRB5_CFLAGS@ \
	@KRB525_CFLAGS@	


momctl_LDADD = \
	$(top_builddir)/src/lib/Libtpp/libtpp.a \
	$(top_builddir)/src/lib/Liblog/liblog.a \
	$(top_builddir)/src/lib/Libnet/libnet.a \
	$(top_builddir)/src/lib/Libpbs/.libs/libpbs.a \
	$(top_builddir)/src/lib/Libutil/libutil.a \
	-lcrypto \
	-lpthread \
	@KRB5_LIBS@ \
	@KRB525_LIBS@	

momctl_SOURCES = \
	momctl.c

