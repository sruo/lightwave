bin_PROGRAMS = ic-join

ic_join_SOURCES = \
    main.c

ic_join_CPPFLAGS = \
    -I$(top_srcdir)/include \
    -I$(top_srcdir)/include/public \
    @LW_INCLUDES@

ic_join_LDADD = \
    @top_builddir@/cfgutils/libcfgutils.la \
    @LWSM_LIBS@ \
    @LWADVAPI_LIBS@ \
    @LWIO_LIBS@ \
    @SCHANNEL_LIBS@ \
    @LWMSG_LIBS@ \
    @LWREG_LIBS@ \
    @LWBASE_LIBS@ \
    @CRYPTO_LIBS@ \
    @GSSAPI_LIBS@ \
    @UUID_LIBS@ \
    @LDAP_LIBS@ \
    @PTHREAD_LIBS@

ic_join_LDFLAGS = \
    @VMCA_LDFLAGS@ \
    @VMAFD_LDFLAGS@ \
    @VMDIR_LDFLAGS@ \
    @LW_LDFLAGS@ \
    @OPENSSL_LDFLAGS@
