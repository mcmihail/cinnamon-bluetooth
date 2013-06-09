include Makefile-lib.am

bin_PROGRAMS = cinnamon-bluetooth-applet

cinnamon_bluetooth_applet_SOURCES = main.c notify.h notify.c agent.h agent.c

cinnamon_bluetooth_applet_LDADD = $(top_builddir)/lib/libcinnamon-bluetooth.la $(builddir)/libcinnamon-bluetooth-applet.la $(APPLET_LIBS)

noinst_PROGRAMS = test-agentdialog test-icon

test_agentdialog_SOURCES = test-agentdialog.c notify.h notify.c
test_agentdialog_LDADD = $(builddir)/libcinnamon-bluetooth-applet.la $(APPLET_LIBS)

test_icon_SOURCES = test-icon.c notify.h notify.c
test_icon_LDADD = $(APPLET_LIBS)

AM_CFLAGS += $(APPLET_CFLAGS) $(WARN_CFLAGS) $(DISABLE_DEPRECATED) -DPKGDATADIR="\"$(pkgdatadir)\"" -DG_LOG_DOMAIN=\"CinnamonBluetoothApplet\"

INCLUDES = -I$(top_srcdir)/lib

if HAVE_APP_INDICATOR
cinnamon_bluetooth_applet_LDADD += $(APP_INDICATOR_LIBS)
test_agentdialog_LDADD += $(APP_INDICATOR_LIBS)
test_icon_LDADD += $(APP_INDICATOR_LIBS)
INCLUDES += -DHAVE_APP_INDICATOR $(APP_INDICATOR_CFLAGS)
endif

man_MANS = cinnamon-bluetooth-applet.1

ui_DATA = popup-menu.ui authorisation-dialogue.ui confirm-dialogue.ui passkey-dialogue.ui
uidir = $(pkgdatadir)

autostartdir = $(sysconfdir)/xdg/autostart

autostart_in_in_files = cinnamon-bluetooth-applet.desktop.in.in
autostart_in_files = cinnamon-bluetooth-applet.desktop.in

autostart_DATA = $(autostart_in_files:.desktop.in=.desktop)

@INTLTOOL_DESKTOP_RULE@

CLEANFILES += $(autostart_DATA)

EXTRA_DIST += $(man_MANS) $(autostart_in_in_files) $(ui_DATA)

MAINTAINERCLEANFILES = Makefile.in

