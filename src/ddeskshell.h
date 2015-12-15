#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <locale.h>
#include <libconfig.h>
#include <libmm-glib.h>
#include <fcntl.h>

#include <nm-client.h>
#include <nm-device.h>
#include <nm-device-wifi.h>
#include <nm-access-point.h>
#include <NetworkManager.h>
#include <nm-utils.h>
#include <nm-device-modem.h>


#define VERSION	"0.1"

//GTK+

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <glib.h>
typedef struct {
	const char * name;
	const char * locale;
	const char * icone;
	GtkWidget  *image;

} ddesk_locale;


typedef struct {
	const char * locale;
	const char * setup_command;
	const char * background_file;
	const char * no_wireless_icone;
	const char * wifi_0_icone;
	const char * wifi_1_icone;
	const char * wifi_2_icone;
	const char * wifi_3_icone;
	const char * wifi_4_icone;
	const char * wifi_5_icone;
	const char * c3g_0_icone;
	const char * c3g_1_icone;
	const char * c3g_2_icone;
	const char * c3g_3_icone;
	const char * c3g_4_icone;
	const char * c3g_5_icone;
	const char * bat_0_icone;
	const char * bat_1_icone;
	const char * bat_2_icone;
	const char * bat_3_icone;
	const char * bat_4_icone;
	const char * bat_5_icone;
	const char * bat_charge_0_icone;
	const char * bat_charge_1_icone;
	const char * bat_charge_2_icone;
	const char * bat_charge_3_icone;
	const char * bat_charge_4_icone;
	const char * bat_charge_5_icone;
	double bat_icone_X;    //0-1.0
	double bat_icone_Y;    //0-1.0
	double net_icone_X;    //0-1.0
	double net_icone_Y;    //0-1.0
	const char * firefox_icone;
	const char * setting_icone;
	double button_box_X;    //0-1.0
	double button_box_Y;    //0-1.0
	double lang_box_X;    //0-1.0
	double lang_box_Y;    //0-1.0

	int window_width;
	int window_hight;
	const ddesk_locale *ddesk_locale_list;
    int ddesk_locale_num;

} App_data;

void load_cfg(void);

