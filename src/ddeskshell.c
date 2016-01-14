#define ddesk_cgf_file ddesk_data_path"/ddesk_shell.cfg"
#define ddesk_cgf_locale_file ddesk_data_path"/locale.cfg"

#include "ddeskshell.h"
void load_cfg(void);
void refresh_main_form(void);
App_data app={0};
int set_any_screen;
int pid_settings=NULL;


//gtk
GtkWidget *window = NULL;
GtkWidget *pic_bat = NULL;
GtkWidget *pic_net = NULL;
GtkWidget *pic_setup = NULL;
GtkWidget *pic_firefox = NULL;

GtkWidget *but_setup = NULL;
GtkWidget *but_firefox = NULL;
GtkWidget *but_box = NULL;


GtkWidget *mainbox = NULL;
GtkWidget *langbox_in = NULL;
//static GtkWidget *draw_area = NULL;


MMManager * ddesk_mmmanager;
NMClient *nm_client;
GDBusProxy *proxy;
GError *error = NULL;
char locale_buf[2]="en";
//--------------------------------------------------------------------------------------------------------------------

void but_firefox_callback(void)
{
//	system("xterm");
exit(2);
}
//--------------------------------------------------------------------------------------------------------------------

void but_setup_callback(void)
{

	int status;
	pid_t result=-1;

	if (pid_settings){
		result = waitpid(pid_settings, &status, WNOHANG);

	if (result == 0) return;

	}
/*
	//if (pid_settings) return;
struct stat fileStat;
char path_buf[20];
int strez=0;
if (pid_settings){
	sprintf(path_buf, "%s%d","/proc/",pid_settings);
	strez=stat(path_buf,&fileStat);
    }
if((pid_settings) && (strez>=0))return; */
pid_settings = fork();
if(!pid_settings)
	{execl(app.setup_command,NULL);  //("/usr/bin/unity-control-center",NULL);
	exit(0);
	}

}
//--------------------------------------------------------------------------------------------------------------------






void load_net_pic_to_widget(void) {
	char str[1024];

	const char * ing_net=app.wifi_0_icone;

	NMActiveConnection * ddesk_NMActiveConnection;
	const GPtrArray * ddesk_GPtrArray;
	NMDevice *device;
	NMAccessPoint *active_ap = NULL;
	ddesk_NMActiveConnection=nm_client_get_primary_connection (nm_client);

	if(	ddesk_NMActiveConnection)
{

	ddesk_GPtrArray=nm_active_connection_get_devices (ddesk_NMActiveConnection);
	device= g_ptr_array_index (ddesk_GPtrArray,0);

	if (NM_IS_DEVICE_WIFI (device))
	{
		/* Get active AP */
		if (nm_device_get_state (device) == NM_DEVICE_STATE_ACTIVATED) {
			if ((active_ap = nm_device_wifi_get_active_access_point (NM_DEVICE_WIFI (device)))) {
			guint8	strength = nm_access_point_get_strength (active_ap);
			int n_pic_bat=(strength+10)/20;
			switch (n_pic_bat) {
			case 0:
				ing_net = app.wifi_0_icone;
				break;
			case 1:
				ing_net = app.wifi_1_icone;
				break;
			case 2:
				ing_net = app.wifi_2_icone;
				break;
			case 3:
				ing_net = app.wifi_3_icone;
				break;
			case 4:
				ing_net = app.wifi_4_icone;
				break;
			case 5:
				ing_net = app.wifi_5_icone;
				break;

			}



			//printf("strength %u\n\n", strength);
			}

		}


	}
	//3gmodem?
	else
	{
		if (NM_IS_DEVICE_MODEM(device)){

			MMModem * ddesk_modem;
			guint strength;
			ing_net=app.c3g_0_icone;
			//printf("strength %s\n\n",nm_device_get_iface (device));
			//GVariant * ddesk_prop=	g_dbus_proxy_get_cached_property (proxy,"SignalQuality");
//			guint8	strength=g_variant_get_byte (ddesk_prop);

			GList *ddesk_list=g_dbus_object_manager_get_objects (ddesk_mmmanager);

			GList *l;
			for (l = ddesk_list; l != NULL; l = l->next)
			  {
				// do something with l->data
				gboolean recent;
				ddesk_modem=mm_object_get_modem (l->data);
				strength=	mm_modem_get_signal_quality (ddesk_modem,&recent);

				const gchar *mod_port=mm_modem_get_primary_port (ddesk_modem);
				const gchar *mod_portNM=nm_device_get_iface(device);

				if(strcmp(mod_port,mod_portNM)==0){
				//printf("Modem: %s : %s - strength %u\n\n",mod_port,mod_portNM,strength);


				}
				//break;
			  }

			int n_pic_net=(strength+10)/20;
			switch (n_pic_net) {
			case 0:
				ing_net = app.c3g_0_icone;
				break;
			case 1:
				ing_net = app.c3g_1_icone;
				break;
			case 2:
				ing_net = app.c3g_2_icone;
				break;
			case 3:
				ing_net = app.c3g_3_icone;
				break;
			case 4:
				ing_net = app.c3g_4_icone;
				break;
			case 5:
				ing_net = app.c3g_5_icone;
				break;

			}


			//	printf("strength %u\n\n",ddesk_prop);
		//	if(ddesk_prop)printf("strength %s\n\n", g_variant_print(ddesk_prop,1));
            g_object_unref(ddesk_modem) ;
			g_list_free(ddesk_list);
		}
		else{
			ing_net = app.no_wireless_icone;
		}
	}

}
	else ing_net = app.no_wireless_icone;

	sprintf(str, "%s/%s", ddesk_data_path, ing_net);
//printf("path_pic_bat : %s\n \n",str);
	gtk_image_set_from_file(GTK_IMAGE(pic_net), str);

}

//--------------------------------------------------------------------------------------------------------------------

void load_bat_pic_to_widget(void) {
	char str[1024];
	const char * ing_bat;
	int if_online;
	int charge_now;
	int charge_full;
	int charge_full_design;
	int n_pic_bat = 0;
	int temp;

	FILE * stream = fopen("/sys/class/power_supply/AC/online", "r");


	if(stream){
	temp = fscanf(stream, "%i", &if_online);
	fclose(stream);
        }



	stream = fopen("/sys/class/power_supply/BAT0/charge_now", "r");
	if (!stream) {
		ing_bat = app.bat_0_icone;
		goto end_load_bat_pic_to_widget;
	}
	temp = fscanf(stream, "%i", &charge_now);
	fclose(stream);

	stream = fopen("/sys/class/power_supply/BAT0/charge_full", "r");
	temp = fscanf(stream, "%i", &charge_full);
	fclose(stream);

	stream = fopen("/sys/class/power_supply/BAT0/charge_full_design", "r");
	temp = fscanf(stream, "%i", &charge_full_design);
	fclose(stream);

	n_pic_bat = charge_now / (charge_full / 5);
	if (charge_now > charge_full)
		n_pic_bat = 5;
//printf(" : %i : %i : %i : %i : \n \n",charge_now,charge_full,charge_full_design,n_pic_bat);
	if (if_online) {
		switch (n_pic_bat) {
		case 0:
			ing_bat = app.bat_charge_0_icone;
			break;
		case 1:
			ing_bat = app.bat_charge_1_icone;
			break;
		case 2:
			ing_bat = app.bat_charge_2_icone;
			break;
		case 3:
			ing_bat = app.bat_charge_3_icone;
			break;
		case 4:
			ing_bat = app.bat_charge_4_icone;
			break;
		case 5:
			ing_bat = app.bat_charge_5_icone;
			break;

		}

	} else {
		switch (n_pic_bat) {
		case 0:
			ing_bat = app.bat_0_icone;
			break;
		case 1:
			ing_bat = app.bat_1_icone;
			break;
		case 2:
			ing_bat = app.bat_2_icone;
			break;
		case 3:
			ing_bat = app.bat_3_icone;
			break;
		case 4:
			ing_bat = app.bat_4_icone;
			break;
		case 5:
			ing_bat = app.bat_5_icone;
			break;

		}

	}
	end_load_bat_pic_to_widget:

//printf("path_pic_bat : %s\n \n",app.bat_4_icone);

	sprintf(str, "%s/%s", ddesk_data_path, ing_bat);
//printf("path_pic_bat : %s\n \n",str);
	gtk_image_set_from_file(GTK_IMAGE(pic_bat), str);

}

//--------------------------------------------------------------------------------------------------------------------

gboolean ddesk_timer(gpointer user_data) {

	load_bat_pic_to_widget();
	load_net_pic_to_widget();
	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------

void load_cfg_error(config_t * cfg, const char * text) {
	fprintf(stderr, "%s:%d - %s  %s\n", config_error_file(cfg),
			config_error_line(cfg), config_error_text(cfg), text);
	config_destroy(cfg);
	exit(EXIT_FAILURE);
}

//--------------------------------------------------------------------------------------------------------------------
void but_lang_callback(GtkButton *b,ddesk_locale * loc)
{



	  /* open config locale file*/

	int loc_conf=open ("ddesklocale.cfg", O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
	if (loc_conf==-1)
		{
		printf("Can not open file %s","ddesklocale.cfg");
		exit(EXIT_FAILURE);
		}
     write(loc_conf,loc->name,2);
     close(loc_conf);
 	load_cfg();

 	refresh_main_form();
 	FILE *loc_en=fopen ("/etc/default/locale", "w");
	if (loc_en==NULL)
		{
		printf("Can not open file %s","/etc/default/locale");
		exit(EXIT_FAILURE);
		}
	ddesk_locale *poiner_loc=app.ddesk_locale_list;
	int i;
    for(i = 0; i < app.ddesk_locale_num; ++i)
    {
       if((poiner_loc->name[0]==app.locale[0]) && (poiner_loc->name[1]==app.locale[1]) && (poiner_loc->locale!=0))
       {
    	  fprintf(loc_en,"LANG=\"%s\"\n",poiner_loc->locale);
    	  fprintf(loc_en,"LANGUAGE=\"%s\"\n",poiner_loc->locale);
    	  fprintf(loc_en,"LC_NUMERIC=\"%s\"\n",poiner_loc->locale);
    	  fprintf(loc_en,"LC_TIME=\"%s\"\n",poiner_loc->locale);
    	  fprintf(loc_en,"LC_MONETARY=\"%s\"\n",poiner_loc->locale);
    	  fprintf(loc_en,"LC_PAPER=\"%s\"\n",poiner_loc->locale);
    	  fprintf(loc_en,"LC_IDENTIFICATION=\"%s\"\n",poiner_loc->locale);
    	  fprintf(loc_en,"LC_NAME=\"%s\"\n",poiner_loc->locale);
    	  fprintf(loc_en,"LC_ADDRESS=\"%s\"\n",poiner_loc->locale);
    	  fprintf(loc_en,"LC_TELEPHONE=\"%s\"\n",poiner_loc->locale);
    	  fprintf(loc_en,"LC_MEASUREMENT=\"%s\"\n",poiner_loc->locale);



       }
      poiner_loc++;
    }
    fclose(loc_en);

}

//--------------------------------------------------------------------------------------------------------------------


gboolean  ddeskcfg_load_str( const char  **dest,config_setting_t *any,config_setting_t *privat,  const char *name)
{
const char* tmp_str;

if(privat){
    if(config_setting_lookup_string(privat,name, &tmp_str)) goto cfg_load_str_Ok;
    }

if(any){
    if(config_setting_lookup_string(any,name, &tmp_str)) goto cfg_load_str_Ok;
    }
	return FALSE;

cfg_load_str_Ok:
	if(*dest)g_free(*dest);
	*dest=g_strdup(tmp_str);
	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------
gboolean  ddeskcfg_load_float(double *dest,config_setting_t *any,config_setting_t *privat,  const char *name)
{
	double tmp;

if(privat){
    if(config_setting_lookup_float(privat,name, &tmp)) goto cfg_load_float_Ok;
    }

if(any){
    if(config_setting_lookup_float(any,name, &tmp)) goto cfg_load_float_Ok;
    }
	return FALSE;

cfg_load_float_Ok:

	*dest=tmp;
	return TRUE;
}

//--------------------------------------------------------------------------------------------------------------------

void load_cfg(void) {


	config_t cfg;
	config_setting_t *setting_any,*setting_private,*setting_main, *locale_list;
	char str[256];
	const char* tmp_str;


	  /* open config file*/
	config_init(&cfg);
	printf("load file: %s\n\n", ddesk_cgf_file);
	if (!config_read_file(&cfg, ddesk_cgf_file))
		load_cfg_error(&cfg, ddesk_cgf_file);

	  /* look up sections*/
	setting_main = config_lookup(&cfg,"main");
	if(!setting_main)load_cfg_error(&cfg, "Do not found section 'main' in config file");


	int loc_conf=open ("ddesklocale.cfg",O_RDONLY);
	if(loc_conf==-1){

	loc_conf=open ("ddesklocale.cfg", O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
	if (loc_conf==-1)
		{
		printf("Can not open file %s",ddesk_cgf_locale_file);
		exit(EXIT_FAILURE);
		}
     write(loc_conf,locale_buf,2);
     close(loc_conf);
	}
	else{
		 read(loc_conf,locale_buf,2);
	     close(loc_conf);
	}
	app.locale=locale_buf;


	if(!ddeskcfg_load_str(&app.setup_command,setting_main,NULL,"setup_command"))load_cfg_error(&cfg, "Do not found  'setup_command' in config file");


	locale_list = config_lookup(&cfg,"locale_list");
	if(!locale_list)load_cfg_error(&cfg, "Do not found section 'locale_list' in config file");

	setting_any = config_lookup(&cfg,"any");
	if(!setting_any)load_cfg_error(&cfg, "Do not found section 'any' in config file");

	sprintf(str, "%s.scr_%i_%i",app.locale, app.window_width, app.window_hight);


	setting_private = config_lookup(&cfg,str);
	if(setting_private)printf(" Private section selected in cinfig  : %s\n \n", str);
	else {
		sprintf(str, "%s.%s",app.locale,"scr_other");
		setting_private = config_lookup(&cfg,str);
		if(setting_private)printf(" Private section selected in cinfig  : %s\n \n", str);
		else printf(" Private section do not selected.");
	}




	  /* load locale list*/


		app.ddesk_locale_num = config_setting_length(locale_list);
	    int i;
        if(app.ddesk_locale_list==0) app.ddesk_locale_list=g_malloc0(sizeof(ddesk_locale)*app.ddesk_locale_num);
	    ddesk_locale *poiner_loc=app.ddesk_locale_list;

	    for(i = 0; i < app.ddesk_locale_num; ++i)
	    {
	      config_setting_t *locale = config_setting_get_elem(locale_list, i);
	      ddeskcfg_load_str(&poiner_loc->name,locale,NULL,"name");
	      ddeskcfg_load_str(&poiner_loc->locale,locale,NULL,"locale");
	      ddeskcfg_load_str(&poiner_loc->icone,setting_any,setting_private,poiner_loc->name);
          if((poiner_loc->name[0]==app.locale[0]) && (poiner_loc->name[1]==app.locale[1]) && (poiner_loc->locale!=0))
        	  {setenv("LANG",poiner_loc->locale,TRUE);
        	  setenv("LANGUAGE",poiner_loc->locale,TRUE);
        	  system("locale");
        	  }
	      poiner_loc++;
	    }






	  ddeskcfg_load_float(&app.bat_icone_X,setting_any,setting_private,"bat_icone_X");
	  ddeskcfg_load_float(&app.bat_icone_Y,setting_any,setting_private,"bat_icone_Y");
	  ddeskcfg_load_float(&app.net_icone_X,setting_any,setting_private,"net_icone_X");
	  ddeskcfg_load_float(&app.net_icone_Y,setting_any,setting_private,"net_icone_Y");
	  ddeskcfg_load_float(&app.button_box_X,setting_any,setting_private,"button_box_X");
	  ddeskcfg_load_float(&app.button_box_Y,setting_any,setting_private,"button_box_Y");
	  ddeskcfg_load_float(&app.lang_box_X,setting_any,setting_private,"language_box_X");
	  ddeskcfg_load_float(&app.lang_box_Y,setting_any,setting_private,"language_box_Y");


	  ddeskcfg_load_str(&app.background_file,setting_any,setting_private,"background_file");
	  ddeskcfg_load_str(&app.bat_charge_0_icone,setting_any,setting_private,"bat_charge_0_icone");
	  ddeskcfg_load_str(&app.bat_charge_1_icone,setting_any,setting_private,"bat_charge_1_icone");
	  ddeskcfg_load_str(&app.bat_charge_2_icone,setting_any,setting_private,"bat_charge_2_icone");
	  ddeskcfg_load_str(&app.bat_charge_3_icone,setting_any,setting_private,"bat_charge_3_icone");
	  ddeskcfg_load_str(&app.bat_charge_4_icone,setting_any,setting_private,"bat_charge_4_icone");
	  ddeskcfg_load_str(&app.bat_charge_5_icone,setting_any,setting_private,"bat_charge_5_icone");
	  ddeskcfg_load_str(&app.bat_0_icone,setting_any,setting_private,"bat_0_icone");
	  ddeskcfg_load_str(&app.bat_1_icone,setting_any,setting_private,"bat_1_icone");
	  ddeskcfg_load_str(&app.bat_2_icone,setting_any,setting_private,"bat_2_icone");
	  ddeskcfg_load_str(&app.bat_3_icone,setting_any,setting_private,"bat_3_icone");
	  ddeskcfg_load_str(&app.bat_4_icone,setting_any,setting_private,"bat_4_icone");
	  ddeskcfg_load_str(&app.bat_5_icone,setting_any,setting_private,"bat_5_icone");
	  ddeskcfg_load_str(&app.wifi_0_icone,setting_any,setting_private,"wifi_0_icone");
	  ddeskcfg_load_str(&app.wifi_1_icone,setting_any,setting_private,"wifi_1_icone");
	  ddeskcfg_load_str(&app.wifi_2_icone,setting_any,setting_private,"wifi_2_icone");
	  ddeskcfg_load_str(&app.wifi_3_icone,setting_any,setting_private,"wifi_3_icone");
	  ddeskcfg_load_str(&app.wifi_4_icone,setting_any,setting_private,"wifi_4_icone");
	  ddeskcfg_load_str(&app.wifi_5_icone,setting_any,setting_private,"wifi_5_icone");
	  ddeskcfg_load_str(&app.c3g_0_icone,setting_any,setting_private,"c3g_0_icone");
	  ddeskcfg_load_str(&app.c3g_1_icone,setting_any,setting_private,"c3g_1_icone");
	  ddeskcfg_load_str(&app.c3g_2_icone,setting_any,setting_private,"c3g_2_icone");
	  ddeskcfg_load_str(&app.c3g_3_icone,setting_any,setting_private,"c3g_3_icone");
	  ddeskcfg_load_str(&app.c3g_4_icone,setting_any,setting_private,"c3g_4_icone");
	  ddeskcfg_load_str(&app.c3g_5_icone,setting_any,setting_private,"c3g_5_icone");
	  ddeskcfg_load_str(&app.no_wireless_icone,setting_any,setting_private,"no_wireless_icone");
	  ddeskcfg_load_str(&app.firefox_icone,setting_any,setting_private,"firefox_icone");
	  ddeskcfg_load_str(&app.setting_icone,setting_any,setting_private,"setting_icone");

	config_destroy(&cfg); /* Освободить память обязательно, если это не конец программы */
	return;

}

//--------------------------------------------------------------------------------------------------------------------

// exit programm;
static void app_exit(GtkWidget *widget, gpointer data) {
	printf("bye!\n");
	gtk_main_quit();
}

//--------------------------------------------------------------------------------------------------------------------
void create_main_window(void) {


	// главное окно
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL); // Главное окно создаем;  GTK_WINDOW_TOPLEVEL GTK_WINDOW_POPUP
	gtk_window_set_modal(GTK_WINDOW(window), TRUE);
	gtk_window_set_hide_titlebar_when_maximized(GTK_WINDOW(window), TRUE);
	gtk_window_fullscreen(GTK_WINDOW(window));

	//will get dispay size
	app.window_width = gdk_screen_get_width(
			gtk_window_get_screen(GTK_WINDOW(window)));
	app.window_hight = gdk_screen_get_height(
			gtk_window_get_screen(GTK_WINDOW(window)));
	//gtk_window_get_size (window,&ddesk_window_width,&ddesk_window_height);

	printf("window_width=%d ? window_height=%d \n", app.window_width,
			app.window_hight);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(app_exit), NULL);

}

//--------------------------------------------------------------------------------------------------------------------

// Создаём главное окно
void create_main_form(void) {
	/*----- CSS ----------- */
	GtkCssProvider *provider;
	GdkDisplay *display;
	GdkScreen *screen;
	/*-----------------------*/




	mainbox = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), mainbox);
//	gtk_box_pack_start(GTK_BOX(mainbox), draw_area, TRUE, TRUE, 0);
	//установка background
		/*------------- CSS  --------------------------------------------------------------------------------------------------*/
		provider = gtk_css_provider_new();
		display = gdk_display_get_default();
		screen = gdk_display_get_default_screen(display);
		/*
		 GtkStyleContext *ddesk_style=	gtk_style_context_new ();
		 GtkWidgetPath * ddesk_widget_path=  gtk_widget_path_new ();
		 gtk_widget_path_append_for_widget (ddesk_widget_path, GTK_WIDGET(window));
		 gtk_style_context_set_path (ddesk_style,
		 ddesk_widget_path);

		 // gtk_style_context_add_provider (ddesk_style,
		 //                               GTK_STYLE_PROVIDER(provider),
		 //                               GTK_STYLE_PROVIDER_PRIORITY_USER);
		 */
		gtk_style_context_add_provider_for_screen(screen,
				GTK_STYLE_PROVIDER(provider),
				GTK_STYLE_PROVIDER_PRIORITY_USER);

		char str[1024], backgroun_path[1024];
		backgroun_path[0] = 0;
		strncat(backgroun_path, ddesk_data_path, 510);
	//printf("path_background_file1 : %s\n \n",backgroun_path);
		strncat(backgroun_path, "/", 1);
		strncat(backgroun_path, app.background_file, 510);
	//printf("background_file : %s\n \n",app.background_file);
	//printf("path_background_file2 : %s\n \n",backgroun_path);
		sprintf(str, " GtkWindow {\n"
				"   background-image: url('%s');\n"
				"   background-repeat:round;\n"
				"}\n\n"
				" GtkButton {\n"
				" border-image: none;\n"
				" background-image: none;\n"
				"   background-color: rgba(100,100, 200, 0.35);\n"
		        "}\n"
				, backgroun_path);
		gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(provider), str, -1, NULL);

		g_object_unref(provider);
		/*----------------------------------------------------------------------------------------------------------------------*/
		but_setup = gtk_button_new ();
		but_firefox = gtk_button_new ();

		pic_bat = gtk_image_new();
		pic_net = gtk_image_new();
		pic_setup = gtk_image_new();
		pic_firefox = gtk_image_new();

		gtk_button_set_image (GTK_BUTTON(but_setup),pic_setup);
		gtk_button_set_image (GTK_BUTTON(but_firefox),pic_firefox);

		gtk_button_set_label (GTK_BUTTON(but_setup),NULL);
		gtk_button_set_label (GTK_BUTTON(but_firefox),NULL);

		gint tempX = (gint) (app.window_width * app.bat_icone_X);
		gint tempY = (gint) (app.window_hight * app.bat_icone_Y);
	//printf("pic_bat X=%i   Y=%i\n \n",tempX,tempY);
		gtk_fixed_put(GTK_FIXED(mainbox), pic_bat, tempX, tempY);

		tempX = (gint) (app.window_width * app.net_icone_X);
		tempY = (gint) (app.window_hight * app.net_icone_Y);
	//printf("pic_bat X=%i   Y=%i\n \n",tempX,tempY);
		gtk_fixed_put(GTK_FIXED(mainbox), pic_net, tempX, tempY);


		but_box=gtk_box_new (GTK_ORIENTATION_HORIZONTAL,20);
		gtk_box_pack_start (but_box,but_setup,TRUE,TRUE,TRUE);
		gtk_box_pack_start (but_box,but_firefox,TRUE,TRUE,TRUE);

	//	gchar str[256];

		sprintf(str, "%s/%s", ddesk_data_path, app.setting_icone);
		gtk_image_set_from_file(GTK_IMAGE(pic_setup), str);

		tempX = (gint) (app.window_width * app.button_box_X);
        const GdkPixbuf * pixbuf =gtk_image_get_pixbuf(pic_setup);
	    int corr=gdk_pixbuf_get_width(pixbuf);
        tempX -= corr+20;
		tempY = (gint) (app.window_hight * app.button_box_Y);
		corr=gdk_pixbuf_get_height(pixbuf);
		tempY -= corr/2;
	//printf("pic_bat X=%i   Y=%i\n \n",tempX,tempY);
		gtk_fixed_put(GTK_FIXED(mainbox), but_box, tempX, tempY);
		gtk_box_set_homogeneous (GTK_BOX(but_box),TRUE);


	    g_signal_connect(GTK_BUTTON(but_setup), "clicked", G_CALLBACK(but_setup_callback), NULL);
	    g_signal_connect(GTK_BUTTON(but_firefox), "clicked", G_CALLBACK(but_firefox_callback), NULL);

     //create language panel

	    langbox_in=gtk_box_new (GTK_ORIENTATION_HORIZONTAL,2);
	    ddesk_locale *poiner_loc=app.ddesk_locale_list;
	    int i;
	    for(i = 0; i < app.ddesk_locale_num; ++i)
	    {
	    	GtkWidget *pic_lang = gtk_image_new();
	    	GtkWidget *but_lang = gtk_button_new ();
	    	gtk_button_set_image (GTK_BUTTON(but_lang),pic_lang);
	    	gtk_button_set_label (GTK_BUTTON(but_lang),NULL);
	    	//gtk_button_set_always_show_image (GTK_BUTTON(but_lang),TRUE;
			gtk_box_pack_start (langbox_in,but_lang,0,0,0);
			poiner_loc->image=pic_lang;
		    g_signal_connect(GTK_BUTTON(but_lang), "clicked", G_CALLBACK(but_lang_callback),poiner_loc);

			poiner_loc++;
	    }

		tempX = (gint) (app.window_hight * app.lang_box_X);
		tempY = (gint) (app.window_hight * app.lang_box_Y);
	//printf("pic_bat X=%i   Y=%i\n \n",tempX,tempY);
		gtk_fixed_put(GTK_FIXED(mainbox),langbox_in, tempX, tempY);
		gtk_box_set_homogeneous (GTK_BOX(langbox_in),TRUE);



}

//--------------------------------------------------------------------------------------------------------------------
void refresh_main_form(void) {
	gchar str[256];

	sprintf(str, "%s/%s", ddesk_data_path, app.setting_icone);
	gtk_image_set_from_file(GTK_IMAGE(pic_setup), str);

	sprintf(str, "%s/%s", ddesk_data_path, app.firefox_icone);
	gtk_image_set_from_file(GTK_IMAGE(pic_firefox), str);

	load_bat_pic_to_widget();
	load_net_pic_to_widget();

    ddesk_locale *poiner_loc=app.ddesk_locale_list;
    int i;
    for(i = 0; i < app.ddesk_locale_num; ++i)
    {
    	sprintf(str, "%s/%s", ddesk_data_path,poiner_loc->icone);
    	gtk_image_set_from_file(GTK_IMAGE(poiner_loc->image), str);
        poiner_loc++;
    }


}

//--------------------------------------------------------------------------------------------------------------------

int main(int argc, char **argv) {



#if !GLIB_CHECK_VERSION (2, 35, 0)
	/* Initialize GType system */
	g_type_init ();
#endif

	/* Get NMClient object */
	nm_client = nm_client_new ();
	if (!nm_client) {
		g_message ("Error: Could not create NMClient: .");
		return EXIT_FAILURE;
	}

	//--------------------------------------------------------------------------------------------------------------------

	/* Create a D-Bus proxy; */
	proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
	                                       G_DBUS_PROXY_FLAGS_NONE,
	                                       NULL,
	                                       "org.freedesktop.ModemManager1",
	                                       "/org/freedesktop/ModemManager1",
	                                       "org.freedesktop.DBas.ObjectManager",
	                                       NULL, &error);
	if (!proxy) {
		g_dbus_error_strip_remote_error (error);
		g_print ("Could not create NetworkManager D-Bus proxy: %s\n", error->message);
		g_error_free (error);
		return 1;
	}

	GDBusConnection *connection=g_dbus_proxy_get_connection(proxy);
	/*GDBusConnection *connection=g_dbus_connection_new_for_address_sync
            ("/org/freedesktop/ModemManager1",
            		G_DBUS_CONNECTION_FLAGS_NONE,
             NULL,
             NULL,
             &error);*/

	if (!connection) {
		g_dbus_error_strip_remote_error (error);
		g_print ("Could not create NetworkManager D-Bus connection: %s\n", error->message);
		g_error_free (error);
		return 1;
	}


	ddesk_mmmanager=mm_manager_new_sync (connection,
			G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE,
	                     NULL,
	                     &error);

	if (!ddesk_mmmanager) {
		g_dbus_error_strip_remote_error (error);
		g_print ("Could not create NetworkManager D-Bus mmmanager: %s\n", error->message);
		g_error_free (error);
		return 1;
	}


//--------------------------------------------------------------------------------------------------------------------

	gtk_init(&argc, &argv);
	create_main_window();
	load_cfg();
	create_main_form();
	refresh_main_form();











	gtk_widget_show_all(window);
	g_timeout_add(1000, ddesk_timer, NULL);
	gtk_main();

}

