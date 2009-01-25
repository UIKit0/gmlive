/*
 * =====================================================================================
 * 
 *       Filename:  confwindow.h
 * 
 *    Description:  config class for gmlive
 * 
 *        Version:  1.0
 *        Created:  2007年12月17日 21时25分41秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 * 
 * =====================================================================================
 */

#ifndef  CONFWINDOW_FILE_HEADER_INC
#define  CONFWINDOW_FILE_HEADER_INC

#include <libglademm/variablesmap.h>
#include "gmlive.h"
class MainWindow;
class ConfWindow: public Gtk::Window
{

	public:
		ConfWindow(MainWindow* parent_);
		~ConfWindow();

		void on_button_save();
		void on_button_cancel();
		bool on_key_press_event(GdkEventKey* ev);
		bool on_delete_event(GdkEventAny* event);

		typedef sigc::signal<void> type_signal_quit;
		type_signal_quit signal_quit()
		{ return signal_quit_; }
	private:
		void save();
		void write_to_GMConf();
		type_signal_quit signal_quit_;
		MainWindow* parent;
		Gnome::Glade::VariablesMap* m_pVariablesMap;
		Glib::ustring m_paramter;
		Glib::ustring m_sopcast_cache;
		Glib::ustring m_mms_cache;
		Glib::ustring m_nslive_delay;
		Glib::ustring m_nslive_cache;
		Glib::ustring m_sopcast_channel;
		Glib::ustring m_oplayer_cmd;
		bool		m_oplayer;
		bool		m_embed;
		bool		m_enable_nslive;
		bool		m_enable_sopcast;
		bool		m_check_refresh_sopcast_channels;

		/*
		   Gtk::CheckButton* check_embed;
		   Gtk::Entry* mplayer_param;
		   Gtk::Entry* mms_mplayer_cache;
		   Gtk::Entry* nslive_mplayer_cache;
		   Gtk::Entry* sopcast_mplayer_cache;
		   Gtk::Entry* nslive_delay_time;
		   */


};
#endif   /* ----- #ifndef CONFWINDOW_FILE_HEADER_INC  ----- */

