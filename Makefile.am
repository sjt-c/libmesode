AUTOMAKE_OPTIONS = subdir-objects

ACLOCAL_AMFLAGS = -I m4

PARSER_CFLAGS=@PARSER_CFLAGS@
PARSER_LIBS=@PARSER_LIBS@

SSL_CFLAGS = @openssl_CFLAGS@
SSL_LIBS = @openssl_LIBS@

RESOLV_LIBS = @RESOLV_LIBS@

WARNING_FLAGS = @WARNING_FLAGS@
MESODE_FLAGS = -I$(top_srcdir) $(WARNING_FLAGS)
MESODE_LIBS = libmesode.la

## Main build targets
lib_LTLIBRARIES = libmesode.la

libmesode_la_CFLAGS = $(SSL_CFLAGS) $(MESODE_FLAGS) $(PARSER_CFLAGS)
libmesode_la_LDFLAGS = $(SSL_LIBS) $(PARSER_LIBS) $(RESOLV_LIBS) -no-undefined
# Export only public API
libmesode_la_LDFLAGS += -export-symbols-regex '^xmpp_'

libmesode_la_SOURCES = \
	src/auth.c \
	src/conn.c \
	src/crypto.c \
	src/ctx.c \
	src/event.c \
	src/handler.c \
	src/hash.c \
	src/jid.c \
	src/md5.c \
	src/rand.c \
	src/resolver.c \
	src/sasl.c \
	src/scram.c \
	src/sha1.c \
	src/snprintf.c \
	src/sock.c \
	src/socks5.c \
	src/stanza.c \
	src/util.c \
	src/uuid.c
libmesode_la_SOURCES += \
	src/common.h \
	src/hash.h \
	src/md5.h \
	src/ostypes.h \
	src/parser.h \
	src/rand.h \
	src/resolver.h \
	src/sasl.h \
	src/scram.h \
	src/sha1.h \
	src/snprintf.h \
	src/sock.h \
	src/socks5.h \
	src/tls.h \
	src/util.h

if DISABLE_TLS
libmesode_la_SOURCES += src/tls_dummy.c
else
libmesode_la_SOURCES += src/tls_openssl.c
endif

libmesode_la_SOURCES += src/parser_expat.c

include_HEADERS = mesode.h

pkgconfig_DATA = libmesode.pc

EXTRA_DIST = \
	Doxyfile \
	GPL-LICENSE.txt \
	LICENSE.txt \
	MIT-LICENSE.txt \
	bootstrap.sh \
	build-android.sh \
	docs/footer.html \
	examples/README.md \
	jni/Android.mk \
	jni/Application.mk \
	tests/res_query_dump.c

## Examples
noinst_PROGRAMS = \
	examples/active \
	examples/basic \
	examples/bot \
	examples/component \
	examples/roster \
	examples/uuid \
	examples/vcard

examples_active_SOURCES = examples/active.c
examples_active_CFLAGS = $(MESODE_FLAGS)
examples_active_LDADD = $(MESODE_LIBS) 
examples_basic_SOURCES = examples/basic.c
examples_basic_CFLAGS = $(MESODE_FLAGS)
examples_basic_LDADD = $(MESODE_LIBS) 
examples_bot_SOURCES = examples/bot.c
examples_bot_CFLAGS = $(MESODE_FLAGS)
examples_bot_LDADD = $(MESODE_LIBS)
examples_component_SOURCES = examples/component.c
examples_component_CFLAGS = $(MESODE_FLAGS)
examples_component_LDADD = $(MESODE_LIBS)
examples_roster_SOURCES = examples/roster.c
examples_roster_CFLAGS = $(MESODE_FLAGS)
examples_roster_LDADD = $(MESODE_LIBS) 
examples_uuid_SOURCES = examples/uuid.c
examples_uuid_CFLAGS = $(MESODE_FLAGS)
examples_uuid_LDADD = $(MESODE_LIBS)
examples_vcard_SOURCES = examples/vcard.c
examples_vcard_CFLAGS = $(MESODE_FLAGS)
examples_vcard_LDADD = $(MESODE_LIBS)

## Tests
TESTS = \
	tests/check_parser \
	tests/test_sha1 \
	tests/test_md5 \
	tests/test_rand \
	tests/test_scram \
	tests/test_ctx \
	tests/test_base64 \
	tests/test_hash \
	tests/test_jid \
	tests/test_snprintf \
	tests/test_string \
	tests/test_resolver

check_PROGRAMS = $(TESTS)

tests_check_parser_SOURCES = tests/check_parser.c tests/test.h
tests_check_parser_CFLAGS = $(PARSER_CFLAGS) $(MESODE_FLAGS) \
	-I$(top_srcdir)/src
tests_check_parser_LDADD = $(MESODE_LIBS)
tests_check_parser_LDFLAGS = -static

tests_test_ctx_SOURCES = tests/test_ctx.c
tests_test_ctx_CFLAGS = $(MESODE_FLAGS) -I$(top_srcdir)/src
tests_test_ctx_LDADD = $(MESODE_LIBS)
tests_test_ctx_LDFLAGS = -static

tests_test_base64_SOURCES = tests/test_base64.c tests/test.c tests/test.h
tests_test_base64_CFLAGS = $(MESODE_FLAGS) -I$(top_srcdir)/src
tests_test_base64_LDADD = $(MESODE_LIBS)
tests_test_base64_LDFLAGS = -static

tests_test_hash_SOURCES = tests/test_hash.c
tests_test_hash_CFLAGS = $(MESODE_FLAGS) -I$(top_srcdir)/src
tests_test_hash_LDADD = $(MESODE_LIBS)
tests_test_hash_LDFLAGS = -static

tests_test_jid_SOURCES = tests/test_jid.c
tests_test_jid_CFLAGS = $(MESODE_FLAGS) -I$(top_srcdir)/src
tests_test_jid_LDADD = $(MESODE_LIBS)
tests_test_jid_LDFLAGS = -static

tests_test_resolver_SOURCES = tests/test_resolver.c tests/test.h
tests_test_resolver_CFLAGS = $(MESODE_FLAGS) -I$(top_srcdir)/src
tests_test_resolver_LDADD = $(MESODE_LIBS)
tests_test_resolver_LDFLAGS = -static

tests_test_rand_SOURCES = tests/test_rand.c tests/test.c src/sha1.c
tests_test_rand_CFLAGS = $(MESODE_FLAGS) -I$(top_srcdir)/src

tests_test_scram_SOURCES = tests/test_scram.c tests/test.c src/sha1.c
tests_test_scram_CFLAGS = $(MESODE_FLAGS) -I$(top_srcdir)/src

tests_test_sha1_SOURCES = tests/test_sha1.c src/sha1.c
tests_test_sha1_CFLAGS = -I$(top_srcdir)/src

tests_test_md5_SOURCES = tests/test_md5.c tests/test.c src/md5.c
tests_test_md5_CFLAGS = -I$(top_srcdir)/src

tests_test_snprintf_SOURCES = tests/test_snprintf.c
tests_test_snprintf_CFLAGS = -I$(top_srcdir)/src

tests_test_string_SOURCES = tests/test_string.c tests/test.h
tests_test_string_CFLAGS = $(MESODE_FLAGS) -I$(top_srcdir)/src
tests_test_string_LDADD = $(MESODE_LIBS)
tests_test_string_LDFLAGS = -static
