/*
 * =====================================================================================
 * 
 *       Filename:  mmschannel.h
 * 
 *    Description:  support ordinarily mms stream like mms://xxx
 * 
 *        Version:  1.0
 *        Created:  2007年12月02日 09时12分37秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 * 
 * =====================================================================================
 */

#ifndef  MMSCHANNEL_FILE_HEADER_INC
#define  MMSCHANNEL_FILE_HEADER_INC

#include "channel.h"
class GMplayer;
class MMSChannel:public Channel
{
	public:
		MMSChannel(MainWindow* parent_);
		~MMSChannel(){}
		void init();
	protected:
		LivePlayer* get_player(const std::string& stream,TypeChannel page);
		void  addLine(const int users,const Glib::ustring& name,const std::string& sream,const Glib::ustring& groupname);

};

#endif   /* ----- #ifndef MMSCHANNEL_FILE_HEADER_INC  ----- */

