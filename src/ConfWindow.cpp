/*
 * =====================================================================================
 *
 *       Filename:  ConfWindow.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2007年12月17日 21时49分13秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */

#include "ConfWindow.h"
#include "MainWindow.h"
#include <fstream>

ConfWindow::ConfWindow(MainWindow * parent_):parent(parent_)
{
	GlademmXML vbox_xml =
	    Gnome::Glade::Xml::create(main_ui, "vbox_conf");
	Gtk::VBox * vBox = dynamic_cast < Gtk::VBox * >
	    (vbox_xml->get_widget("vbox_conf"));

	std::string& oplayer = GMConf["player_type"];
	m_oplayer = (!oplayer.empty())&&(oplayer[0]=='1');
	m_oplayer_cmd = GMConf["other_player_cmd"];

	std::string& embed = GMConf["mplayer_embed"];
	m_embed = (!embed.empty()) && (embed[0] == '1');
	std::string enable = GMConf["enable_nslive"];
	m_enable_nslive = (!enable.empty())&&(enable[0] == '1' );
		enable = GMConf["enable_sopcast"];
	m_enable_sopcast = (enable[0] == '1' );

	m_paramter=GMConf["mplayer_paramter"];
	m_mms_cache=GMConf["mms_mplayer_cache"];
	m_sopcast_cache = GMConf["sopcast_mplayer_cache"];
	m_nslive_cache = GMConf["nslive_mplayer_cache"];
	m_nslive_delay = GMConf["nslive_delay_time"];
	m_sopcast_channel = GMConf["sopcast_channel_url"];


	m_pVariablesMap = new Gnome::Glade::VariablesMap(vbox_xml);
	m_pVariablesMap->connect_widget("rbtn_oplayer",m_oplayer);
	m_pVariablesMap->connect_widget("entry_oplayer_cmd",m_oplayer_cmd);
	m_pVariablesMap->connect_widget("check_embed",m_embed);
	m_pVariablesMap->connect_widget("enable_sopcast", m_enable_sopcast);
	m_pVariablesMap->connect_widget("enable_nslive", m_enable_nslive);
	m_pVariablesMap->connect_widget("entry_parameter", m_paramter);
	m_pVariablesMap->connect_widget("entry_mms_cache", m_mms_cache);
	m_pVariablesMap->connect_widget("entry_nslive_cache",m_nslive_cache);
	m_pVariablesMap->connect_widget("entry_nslive_delay", m_nslive_delay);
	m_pVariablesMap->connect_widget("entry_sopcast_cache",m_sopcast_cache);
	m_pVariablesMap->connect_widget("entry_sopcast_channel",m_sopcast_channel);



	vbox_xml->connect_clicked("button_save", sigc::mem_fun(*this,&ConfWindow::on_button_save));
	vbox_xml->connect_clicked("button_cancel", sigc::mem_fun(*this, &ConfWindow::on_button_cancel));

	m_pVariablesMap->transfer_variables_to_widgets();
	add(*vBox);
	set_transient_for(*parent);


	set_default_size(600, 400);
	set_title(_("GMLive Conf Window"));
	show_all();
}

ConfWindow::~ConfWindow()
{
	delete m_pVariablesMap;
}

void ConfWindow::on_button_save()
{
	write_to_GMConf();
	signal_quit_.emit();
	on_button_cancel();
}


void ConfWindow::on_button_cancel()
{
	//delete this;
	parent->on_conf_window_close(this);
}

void ConfWindow::write_to_GMConf()
{
	m_pVariablesMap->transfer_widgets_to_variables();
	
	GMConf["player_type"] = m_oplayer ? "1" : "0";
	GMConf["mplayer_embed"] = m_embed ? "1" : "0";
	GMConf["enable_nslive"] = m_enable_nslive ? "1" : "0";
	GMConf["enable_sopcast"] = m_enable_sopcast ? "1" : "0";
	GMConf["mplayer_paramter"]      =            m_paramter   ; 
	GMConf["mms_mplayer_cache"]     =            m_mms_cache  ;
	GMConf["sopcast_mplayer_cache"] =            m_sopcast_cache;
	GMConf["nslive_mplayer_cache"]  =            m_nslive_cache ;
	GMConf["nslive_delay_time"]     =            m_nslive_delay ;
	GMConf["sopcast_channel_url"]    =            m_sopcast_channel;
	GMConf["other_player_cmd"]  = m_oplayer_cmd;
}


bool ConfWindow::on_key_press_event(GdkEventKey * ev)
{
	if (ev->type != GDK_KEY_PRESS)
		return Gtk::Window::on_key_press_event(ev);
	switch (ev->keyval) {
		case GDK_Escape:
			on_button_cancel();
			break;
		default:
			return Gtk::Window::on_key_press_event(ev);
	}
	return true;
}

bool ConfWindow::on_delete_event(GdkEventAny* ev)
{
	on_button_cancel();
	return true;
}

