/*
 * =====================================================================================
 * 
 *       Filename:  recentchannel.h
 * 
 *    Description:  最近播放列表
 * 
 *        Version:  1.0
 *        Created:  2007年12月02日 20时17分27秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 * 
 * =====================================================================================
 */

#ifndef  RECENTCHANNEL_FILE_HEADER_INC
#define  RECENTCHANNEL_FILE_HEADER_INC

#include "channel.h"
class GMplayer;
class RecentChannel:public Channel
{
	public:
		RecentChannel(MainWindow* parent_);
		~RecentChannel();
		LivePlayer* get_player(GMplayer& gmp, const std::string& stream,TypeChannel page);
		void init();
		/** 写进文件中*/
		void saveLine(const Glib::ustring& name,const std::string& sream,TypeChannel type);
	protected:
		/** 只添加进列表中*/
		void  addLine(const int users,const Glib::ustring& name,const std::string& sream,const Glib::ustring& type);
	//	void play_selection();
	//	void record_selection();

};
#endif   /* ----- #ifndef RECENTCHANNEL_FILE_HEADER_INC  ----- */

