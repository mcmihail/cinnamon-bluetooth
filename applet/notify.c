/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2005-2008  Marcel Holtmann <marcel@holtmann.org>
 *  Copyright (C) 2006-2007  Bastien Nocera <hadess@hadess.net>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <libnotify/notify.h>
#ifdef HAVE_APP_INDICATOR
#include <libappindicator/app-indicator.h>
#endif /* HAVE_APP_INDICATOR */
#include "notify.h"

#define ACTIVE_ICON_NAME "cinnamon-bluetooth-active"
#define DISABLE_ICON_NAME "cinnamon-bluetooth-disabled"

#ifdef HAVE_APP_INDICATOR
static AppIndicator *indicator = NULL;
#else
static GtkStatusIcon *statusicon = NULL;
#endif /* HAVE_APP_INDICATOR */
static gboolean bt_enabled = FALSE;
static GIcon *icon_enabled = NULL, *icon_disabled = NULL;
static char *tooltip = NULL;
static NotifyNotification *notify = NULL;

static void notify_action(NotifyNotification *notify,
					gchar *action, gpointer user_data)
{
}

gboolean notification_supports_actions (void)
{
	gboolean supports_actions = FALSE;
	GList *caps = NULL;

	caps = notify_get_server_caps ();
	if (g_list_find_custom(caps, "actions", (GCompareFunc)g_strcmp0) != NULL)
		supports_actions = TRUE;

	g_list_foreach(caps, (GFunc)g_free, NULL);
	g_list_free (caps);

	return supports_actions;
}

void show_notification(const gchar *summary, const gchar *message,
			const gchar *action, gint timeout, GCallback handler)
{
	NotifyActionCallback callback;
#ifndef HAVE_APP_INDICATOR
	GdkScreen *screen;
	GdkRectangle area;
#endif /* HAVE_APP_INDICATOR */

	if (notify_is_initted() == FALSE)
		return;

	if (notify) {
		g_signal_handlers_destroy(notify);
		notify_notification_close(notify, NULL);
	}

	notify = notify_notification_new(summary, message, ACTIVE_ICON_NAME);

	notify_notification_set_timeout(notify, timeout);

#ifndef HAVE_APP_INDICATOR
	if (gtk_status_icon_get_visible(statusicon) == TRUE) {
		gtk_status_icon_get_geometry(statusicon, &screen, &area, NULL);

		notify_notification_set_hint_int32(notify,
					"x", area.x + area.width / 2);
		notify_notification_set_hint_int32(notify,
					"y", area.y + area.height / 2);
	}
#endif /* HAVE_APP_INDICATOR */

	notify_notification_set_urgency(notify, NOTIFY_URGENCY_NORMAL);

	callback = handler ? NOTIFY_ACTION_CALLBACK(handler) : notify_action;

	notify_notification_add_action(notify, "default", "action",
						callback, NULL, NULL);
	if (action != NULL)
		notify_notification_add_action(notify, "button", action,
							callback, NULL, NULL);

	notify_notification_show(notify, NULL);
}

void close_notification(void)
{
	if (notify) {
		g_signal_handlers_destroy(notify);
		notify_notification_close(notify, NULL);
		notify = NULL;
	}
}

#ifdef HAVE_APP_INDICATOR
AppIndicator *init_notification(void)
{
	notify_init("bluetooth-manager");

	indicator = app_indicator_new ("bluetooth-manager", 
	                               bt_enabled ? ACTIVE_ICON_NAME
	                                          : DISABLE_ICON_NAME,
	                               APP_INDICATOR_CATEGORY_HARDWARE);

	set_icon (bt_enabled);

	/* TODO: Change this to an appropriate icon/desc. */
	app_indicator_set_attention_icon_full (indicator, "bluetooth-manager",
	                                       _("Bluetooth"));

	return indicator;
}
#else
GtkStatusIcon *init_notification(void)
{
	notify_init("bluetooth-manager");

	icon_enabled = g_themed_icon_new_with_default_fallbacks (ACTIVE_ICON_NAME"-symbolic");
	icon_disabled = g_themed_icon_new_with_default_fallbacks (DISABLE_ICON_NAME"-symbolic");

	statusicon = gtk_status_icon_new_from_gicon(bt_enabled ? icon_enabled : icon_disabled);
	gtk_status_icon_set_title (GTK_STATUS_ICON (statusicon),
				   _("Bluetooth"));
	gtk_status_icon_set_tooltip_markup(statusicon, tooltip);

	/* XXX: Make sure the status icon is actually shown */
	gtk_status_icon_set_visible(statusicon, FALSE);
	gtk_status_icon_set_visible(statusicon, TRUE);

	return statusicon;
}
#endif /* HAVE_APP_INDICATOR */

void cleanup_notification(void)
{
	close_notification();

#ifdef HAVE_APP_INDICATOR
	g_object_unref (indicator);
#else
	g_object_unref (statusicon);
#endif /* HAVE_APP_INDICATOR */
	g_object_unref (icon_enabled);
	g_object_unref (icon_disabled);
	g_free (tooltip);
	tooltip = NULL;

	notify_uninit();
}

void show_icon(void)
{
#ifdef HAVE_APP_INDICATOR
	if (indicator != NULL)
		app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
#else
	if (statusicon != NULL)
		gtk_status_icon_set_visible(statusicon, TRUE);
#endif /* HAVE_APP_INDICATOR */
}

void hide_icon(void)
{
#ifdef HAVE_APP_INDICATOR
	if (indicator != NULL)
		app_indicator_set_status(indicator, APP_INDICATOR_STATUS_PASSIVE);
#else
	if (statusicon != NULL)
		gtk_status_icon_set_visible(statusicon, FALSE);
#endif /* HAVE_APP_INDICATOR */
}

void set_icon(gboolean enabled)
{
	const char *_tooltip = enabled ? _("Bluetooth: On") : _("Bluetooth: Off");

	bt_enabled = enabled;

#ifdef HAVE_APP_INDICATOR
	if (indicator == NULL) {
#else
	if (statusicon == NULL) {
#endif /* HAVE_APP_INDICATOR */
		g_free (tooltip);
		tooltip = g_strdup (_tooltip);
	} else {
#ifdef HAVE_APP_INDICATOR
		app_indicator_set_icon_full(indicator,
		                            bt_enabled ? ACTIVE_ICON_NAME
		                                       : DISABLE_ICON_NAME,
		                            _tooltip);
#else
		gtk_status_icon_set_from_gicon (statusicon, enabled ? icon_enabled : icon_disabled);
		gtk_status_icon_set_tooltip_markup(statusicon, _tooltip);
#endif /* HAVE_APP_INDICATOR */
	}
}
