include Makefile-lib.am

bin_PROGRAMS = bluetooth-applet

bluetooth_applet_SOURCES = main.c notify.h notify.c agent.h agent.c

bluetooth_applet_LDADD = $(top_builddir)/lib/libgnome-bluetooth.la $(builddir)/libgnome-bluetooth-applet.la $(APPLET_LIBS)

noinst_PROGRAMS = test-agentdialog test-icon

test_agentdialog_SOURCES = test-agentdialog.c notify.h notify.c
test_agentdialog_LDADD = $(builddir)/libgnome-bluetooth-applet.la $(APPLET_LIBS)

test_icon_SOURCES = test-icon.c notify.h notify.c
test_icon_LDADD = $(APPLET_LIBS)

AM_CFLAGS += $(APPLET_CFLAGS) $(WARN_CFLAGS) $(DISABLE_DEPRECATED) -DPKGDATADIR="\"$(pkgdatadir)\"" -DG_LOG_DOMAIN=\"BluetoothApplet\"

INCLUDES = -I$(top_srcdir)/lib

if HAVE_APP_INDICATOR
bluetooth_applet_LDADD += $(APP_INDICATOR_LIBS)
test_agentdialog_LDADD += $(APP_INDICATOR_LIBS)
test_icon_LDADD += $(APP_INDICATOR_LIBS)
INCLUDES += -DHAVE_APP_INDICATOR $(APP_INDICATOR_CFLAGS)
endif

man_MANS = bluetooth-applet.1

ui_DATA = popup-menu.ui authorisation-dialogue.ui confirm-dialogue.ui passkey-dialogue.ui
uidir = $(pkgdatadir)

autostartdir = $(sysconfdir)/xdg/autostart

autostart_in_in_files = bluetooth-applet.desktop.in.in
autostart_in_files = bluetooth-applet.desktop.in

autostart_DATA = $(autostart_in_files:.desktop.in=.desktop)

@INTLTOOL_DESKTOP_RULE@

CLEANFILES += $(autostart_DATA)

EXTRA_DIST += $(man_MANS) $(autostart_in_in_files) $(ui_DATA)

MAINTAINERCLEANFILES = Makefile.in

if HAVE_APP_INDICATOR
gsettingsschema_in_files = com.canonical.indicator.bluetooth.gschema.xml.in
gsettings_SCHEMAS = $(gsettingsschema_in_files:.xml.in=.xml)
@INTLTOOL_XML_NOMERGE_RULE@
@GSETTINGS_RULES@
EXTRA_DIST += $(gsettingsschema_in_files)
CLEANFILES += $(gsettings_SCHEMAS)
endif
