/*
 * =====================================================================================
 * 
 *       Filename:  sopcastchannel.h
 * 
 *    Description:  sopcast的列表支持
 * 
 *        Version:  1.0
 *        Created:  2007年12月04日 20时11分23秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 * 
 * =====================================================================================
 */

#ifndef  SOPCASTCHANNEL_FILE_HEADER_INC
#define  SOPCASTCHANNEL_FILE_HEADER_INC
#include "channel.h"
class MainWindow;
class SopcastChannel:public Channel
{
	public:
		SopcastChannel(MainWindow* parent_);
		void init();
		void  addLine(const int id,const Glib::ustring& name,const std::string& sream,const Glib::ustring& groupname);
		void play_selection();
		void record_selection();
		void store_selection();
	private:
		MainWindow* parent;
	protected:
		bool on_button_press_event(GdkEventButton *);
};



#endif   /* ----- #ifndef SOPCASTCHANNEL_FILE_HEADER_INC  ----- */

