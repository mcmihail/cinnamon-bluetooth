SUBDIRS = plugins

EXTRA_DIST = 
CLEANFILES =

lib_LTLIBRARIES = libcinnamon-bluetooth.la

# will be scanned for introspection annotation, but won't be installed
libcinnamon_bluetooth_c_sources =		\
	bluetooth-client.c		\
	bluetooth-utils.c		\
	bluetooth-agent.c		\
	bluetooth-killswitch.c		\
	cinnamon-bluetooth-enum-types.c	\
	bluetooth-chooser.c		\
	bluetooth-filter-widget.c	\
	bluetooth-chooser-button.c	\
	bluetooth-chooser-combo.c	\
	bluetooth-client-glue.c		\
	bluetooth-plugin-manager.c

libcinnamon_bluetooth_private_headers =	\
	bluetooth-client-private.h	\
	bluetooth-agent.h		\
	cinnamon-bluetooth-enum-types.h	\
	bluetooth-chooser-private.h	\
	bluetooth-client-glue.h		\
	rfkill.h

# public headers don't need to be listed, are handled by _HEADERS
libcinnamon_bluetooth_la_SOURCES =			\
	$(libcinnamon_bluetooth_private_headers)	\
	$(libcinnamon_bluetooth_c_sources)

libcinnamon_bluetooth_la_LIBADD = $(LIBCINNAMONBT_LIBS)

libcinnamon_bluetooth_la_LDFLAGS =				\
	-version-info $(CINNAMONBT_LT_VERSION)		\
	-no-undefined					\
	$(AM_LDFLAGS)					\
	-export-symbols $(srcdir)/cinnamon-bluetooth.symbols

libcinnamon_bluetooth_introspect_headers =			\
	bluetooth-client.h				\
	bluetooth-chooser.h				\
	bluetooth-chooser-button.h			\
	bluetooth-chooser-combo.h			\
	bluetooth-filter-widget.h			\
	bluetooth-enums.h				\
	bluetooth-utils.h				\
	bluetooth-plugin-manager.h			\
	bluetooth-killswitch.h

cinnamonbluetoothdir = $(pkgincludedir)
cinnamonbluetooth_HEADERS =				\
	$(libcinnamon_bluetooth_introspect_headers)	\
	bluetooth-plugin.h

AM_CFLAGS =						\
	-I$(srcdir)					\
	$(LIBCINNAMONBT_CFLAGS)				\
	$(WARN_CFLAGS)					\
	$(DISABLE_DEPRECATED)				\
	-DPLUGINDIR=\"$(libdir)/cinnamon-bluetooth/plugins\" \
	-DG_LOG_DOMAIN=\"Bluetooth\"

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = --warn-all --add-include-path=$(srcdir)
INTROSPECTION_COMPILER_ARGS = --includedir=$(srcdir)

if HAVE_INTROSPECTION
introspection_files =				 \
	$(libcinnamon_bluetooth_introspect_headers) \
	$(libcinnamon_bluetooth_c_sources)

CinnamonBluetooth-1.0.gir: libcinnamon-bluetooth.la
CinnamonBluetooth_1_0_gir_INCLUDES = GObject-2.0 Gtk-3.0 GModule-2.0
CinnamonBluetooth_1_0_gir_PACKAGES = gobject-2.0 gtk+-3.0 gmodule-2.0 glib-2.0
CinnamonBluetooth_1_0_gir_CFLAGS = -I$(srcdir)
CinnamonBluetooth_1_0_gir_LIBS = libcinnamon-bluetooth.la
CinnamonBluetooth_1_0_gir_SCANNERFLAGS = --symbol-prefix=bluetooth_ --identifier-prefix=Bluetooth --pkg-export=cinnamon-bluetooth-1.0
CinnamonBluetooth_1_0_gir_FILES = $(introspection_files)
INTROSPECTION_GIRS += CinnamonBluetooth-1.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelib_DATA)

endif # HAVE_INTROSPECTION

BUILT_SOURCES = bluetooth-client-glue.h \
		bluetooth-client-glue.c \
		cinnamon-bluetooth-enum-types.h \
		cinnamon-bluetooth-enum-types.c

CLEANFILES += $(BUILT_SOURCES)

noinst_PROGRAMS = test-client test-agent test-deviceselection test-plugins test-class test-killswitch

test_client_LDADD = libcinnamon-bluetooth.la $(LIBCINNAMONBT_LIBS)

test_agent_LDADD = libcinnamon-bluetooth.la $(LIBCINNAMONBT_LIBS)

test_deviceselection_LDADD = libcinnamon-bluetooth.la $(LIBCINNAMONBT_LIBS)

test_plugins_LDADD = libcinnamon-bluetooth.la $(LIBCINNAMONBT_LIBS)

test_class_LDADD = libcinnamon-bluetooth.la $(LIBCINNAMONBT_LIBS)

test_killswitch_LDADD = libcinnamon-bluetooth.la $(LIBCINNAMONBT_LIBS)

EXTRA_DIST += bluetooth-client.xml cinnamon-bluetooth.symbols

MAINTAINERCLEANFILES = Makefile.in

bluetooth-client-glue.c: bluetooth-client-glue.h

bluetooth-client-glue.c bluetooth-client-glue.h: bluetooth-client.xml
	$(AM_V_GEN) $(GDBUS_CODEGEN) --c-namespace= --generate-c-code=bluetooth-client-glue --interface-prefix=org.bluez $<

cinnamon_bluetooth_headers_to_scan_for_enums = bluetooth-enums.h
# Generate the enums source code, with glib-mkenums:
# This is based on the same Makefile.am stuff in pango:
cinnamon_bluetooth_built_headers = cinnamon-bluetooth-enum-types.h
cinnamon_bluetooth_built_cfiles = cinnamon-bluetooth-enum-types.c

cinnamon-bluetooth-enum-types.h: $(cinnamon_bluetooth_headers_to_scan_for_enums) Makefile
	$(AM_V_GEN) (cd $(srcdir) && glib-mkenums \
			--fhead "#ifndef __CINNAMON_BLUETOOTH_ENUM_TYPES_H__\n#define __CINNAMON_BLUETOOTH_ENUM_TYPES_H__\n\n#include <glib-object.h>\n\nG_BEGIN_DECLS\n" \
			--fprod "/* enumerations from \"@filename@\" */\n" \
			--vhead "GType @enum_name@_get_type (void);\n#define BLUETOOTH_TYPE_@ENUMSHORT@ (@enum_name@_get_type())\n" 	\
			--ftail "G_END_DECLS\n\n#endif /* __CINNAMON_BLUETOOTH_ENUM_TYPES_H__ */" \
		$(cinnamon_bluetooth_headers_to_scan_for_enums)) > $@

cinnamon-bluetooth-enum-types.c: $(cinnamon_bluetooth_headers_to_scan_for_enums) Makefile cinnamon-bluetooth-enum-types.h
	$(AM_V_GEN) (cd $(srcdir) && glib-mkenums \
			--fhead "#include <bluetooth-enums.h>\n" \
			--fhead "#include \"cinnamon-bluetooth-enum-types.h\"\n" \
			--fhead "#include <glib-object.h>" \
		      	--fprod "\n/* enumerations from \"@filename@\" */" \
			--vhead "GType\n@enum_name@_get_type (void)\n{\n  static GType etype = 0;\n  if (etype == 0) {\n    static const G@Type@Value values[] = {" 	\
			--vprod "      { @VALUENAME@, \"@VALUENAME@\", \"@valuenick@\" }," \
			--vtail "      { 0, NULL, NULL }\n    };\n    etype = g_@type@_register_static (\"@EnumName@\", values);\n  }\n  return etype;\n}\n" \
		$(cinnamon_bluetooth_headers_to_scan_for_enums)) > $@
