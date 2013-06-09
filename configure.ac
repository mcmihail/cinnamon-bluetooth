AC_PREREQ(2.52)

m4_define(cinnamonbt_version_major, 1)
m4_define(cinnamonbt_version_minor, 9)
m4_define(cinnamonbt_version_micro, 0)

AC_INIT([cinnamon-bluetooth],
        [cinnamonbt_version_major.cinnamonbt_version_minor.cinnamonbt_version_micro],
        [http://bugzilla.gnome.org/enter_bug.cgi?product=gnome-bluetooth])

AC_CONFIG_SRCDIR([applet/main.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.8 dist-xz no-dist-gzip check-news tar-ustar])

# Before making a release, the CINNAMONBT_LT_VERSION string should be modified.
# The string is of the form C:R:A.
# - If interfaces have been changed or added, but binary compatibility has
#   been preserved, change to C+1:0:A+1
# - If binary compatibility has been broken (eg removed or changed interfaces)
#   change to C+1:0:0
# - If the interface is the same as the previous version, change to C:R+1:A
CINNAMONBT_LT_VERSION=11:0:0
AC_SUBST(CINNAMONBT_LT_VERSION)

AM_MAINTAINER_MODE

# Support silent build rules, requires at least automake-1.11. Disable
# by either passing --disable-silent-rules to configure or passing V=1
# to make
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_LANG_C
AC_PROG_CC
AC_PROG_INSTALL

dnl Initialize libtool
AM_DISABLE_STATIC
AM_PROG_LIBTOOL

# i18n stuff
IT_PROG_INTLTOOL([0.40.0])

GETTEXT_PACKAGE=cinnamon-bluetooth2
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE",
					[The name of the gettext domain])

GLIB_GSETTINGS

AC_PATH_PROG([XMLLINT],[xmllint])

AC_DEFINE_UNQUOTED(LOCALEDIR, "${prefix}/share/locale", [Directory for the localization files])

AC_ARG_ENABLE(desktop-update, AC_HELP_STRING([--disable-desktop-update],
					[Disable desktop database update]))
if (test "$enable_desktop_update" != no); then
	AC_PATH_PROG(UPDATE_DESKTOP_DATABASE, [update-desktop-database])
fi
AM_CONDITIONAL([DESKTOP_UPDATE], [test -n "$UPDATE_DESKTOP_DATABASE"])

AC_ARG_ENABLE(icon-update, AC_HELP_STRING([--disable-icon-update],
					[Disable icon cache update]))
if (test "$enable_icon_update" != no); then
	AC_PATH_PROG(UPDATE_ICON_CACHE, [gtk-update-icon-cache])
fi
AM_CONDITIONAL([ICON_UPDATE], [test -n "$UPDATE_ICON_CACHE"])

GTK_REQUIRED=2.91.3
GLIB_REQUIRED=2.29.90
NOTIFY_REQUIRED=0.7.0

dnl Requires for the applet
PKG_CHECK_MODULES(APPLET,
		  gmodule-export-2.0
		  glib-2.0 >= $GLIB_REQUIRED
		  gtk+-3.0 >= $GTK_REQUIRED
		  libnotify >= $NOTIFY_REQUIRED)

dnl Requires for application indicators
APPINDICATOR_REQUIRED=0.0.7

AC_ARG_ENABLE(appindicator,
                        AS_HELP_STRING([--enable-appindicator[=@<:@no/auto/yes@:>@]],[Build support for application indicators ]),
                        [enable_appindicator=$enableval],
                        [enable_appindicator="auto"])

if test x$enable_appindicator = xauto ; then
        PKG_CHECK_EXISTS([appindicator3-0.1 >= $APPINDICATOR_REQUIRED],
                         enable_appindicator="yes",
                         enable_appindicator="no")
fi

if test x$enable_appindicator = xyes ; then
        PKG_CHECK_EXISTS([appindicator3-0.1 >= $APPINDICATOR_REQUIRED],,
                         AC_MSG_ERROR([appindicator3-0.1 is not installed]))
        PKG_CHECK_MODULES(APP_INDICATOR,
                        appindicator3-0.1 >= $APPINDICATOR_REQUIRED)
        AC_SUBST(APP_INDICATOR_CFLAGS)
        AC_SUBST(APP_INDICATOR_LIBS)
        AC_DEFINE(HAVE_APP_INDICATOR, 1, [Have AppIndicator])
fi
AM_CONDITIONAL(HAVE_APP_INDICATOR, test x"$enable_appindicator" = xyes)

dnl Requires for the wizard dialogue
PKG_CHECK_MODULES(WIZARD,
		  gmodule-export-2.0
		  gtk+-3.0)

AC_CHECK_LIBM
AC_SUBST(LIBM)

dnl Requires for the input helper
PKG_CHECK_MODULES(INPUT,
		  xi
		  x11
		  gtk+-3.0)

dnl Requires for the public library
PKG_CHECK_MODULES(LIBCINNAMONBT, gmodule-2.0 gio-unix-2.0 gtk+-3.0)

dnl Requires for the geoclue plugin
PKG_CHECK_MODULES(GEOCLUE, gio-2.0 gtk+-3.0)

dnl Requires for the plugins
PKG_CHECK_MODULES(PLUGINS,
		  gtk+-3.0)

GDBUS_CODEGEN="gdbus-codegen"
AC_SUBST(GDBUS_CODEGEN)

GOBJECT_INTROSPECTION_CHECK([0.9.5])

GNOME_COMMON_INIT
GNOME_DEBUG_CHECK
GNOME_COMPILE_WARNINGS([maximum])
GNOME_CXX_WARNINGS
GNOME_MAINTAINER_MODE_DEFINES

AC_OUTPUT(Makefile
	  cinnamon-bluetooth-1.0.pc
	  icons/Makefile
	  lib/Makefile
	  lib/plugins/Makefile
	  applet/Makefile
	  applet/bluetooth-applet.desktop.in
	  wizard/Makefile
	  wizard/bluetooth-wizard.desktop.in
	  po/Makefile.in
)

echo "
Configure summary:

	Compiler....................:  ${CC}
	Compiler Flags..............:  ${CFLAGS}
	Prefix......................:  ${prefix}
	GObject-Introspection.......:  ${found_introspection}
	Application Indicators......:  ${enable_appindicator}
"

