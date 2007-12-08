/*
 * =====================================================================================
 *
 *       Filename:  mmschannel.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2007年12月02日 09时16分15秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */


#include <stdlib.h>
#include <fstream>
#include <iostream>
#include "gmlive.h"
#include "mmschannel.h"
#include "MainWindow.h"
#include "mms_live_player.h"

MMSChannel::MMSChannel(MainWindow* parent_):Channel(parent_)
{
	init();
}

LivePlayer* MMSChannel::get_player(GMplayer& gmp, const std::string& stream,TypeChannel page)
{
	return new MmsLivePlayer(gmp, stream);
}

void MMSChannel::init()
{
	char buf[512];
	char* homedir = getenv("HOME");
	snprintf(buf, 512,"%s/.gmlive/mms.lst",homedir);
	std::ifstream file(buf);
	if(!file){
		//char dir[512];
		//snprintf(dir,512,"%s/.gmlive",homedir);
		//mkdir(dir,S_IRUSR|S_IWUSR|S_IXUSR);
		printf("buf is %s\n",buf);
		std::cout<<"file error\n";
		return;
	}
	std::string line;
	std::string name;
	std::string stream;
	std::string groupname;
	std::string last;
	int users=0;
	if(file){
		while(std::getline(file,line)){
			size_t pos = line.find_first_of("#");
			if(pos==std::string::npos)
				continue;
			name = line.substr(0,pos);
			last = line.substr(pos+1,std::string::npos);

			pos = last.find_first_of("#");
			if(pos == std::string::npos)
				continue;
			stream = last.substr(0,pos);
			groupname = last.substr(pos+1,std::string::npos);
			addLine(users,name,stream,groupname);
		}
	}

	file.close();

}

void MMSChannel::addLine(const int num, const Glib::ustring & name,const std::string& stream,const Glib::ustring& groupname)
{
	Gtk::TreeModel::Children children = m_liststore->children();
	Gtk::TreeModel::iterator listiter;
	listiter = getListIter(children,groupname);
	if(listiter == children.end())
		listiter = addGroup(groupname);

	Gtk::TreeModel::iterator iter = m_liststore->append(listiter->children());
	(*iter)[columns.users] = num;
	(*iter)[columns.name] = name;
	(*iter)[columns.freq] = 100;
	(*iter)[columns.stream]=stream;
	(*iter)[columns.type]=MMS_CHANNEL;

}

//void MMSChannel::play_selection()
//{
//	Glib::RefPtr < Gtk::TreeSelection > selection =
//	    this->get_selection();
//	Gtk::TreeModel::iterator iter = selection->get_selected();
//	if (!selection->count_selected_rows())
//		return ;
//	TypeChannel page = (*iter)[columns.type];
//	Glib::ustring name = (*iter)[columns.name];
//	std::string stream = (*iter)[columns.stream];
//
//	parent->play(stream,this);
//	parent->getRecentChannel().saveLine(name,stream,page);
//}

//void MMSChannel::record_selection()
//{
//	Glib::RefPtr < Gtk::TreeSelection > selection =
//	    this->get_selection();
//	Gtk::TreeModel::iterator iter = selection->get_selected();
//	if (!selection->count_selected_rows())
//		return ;
//	TypeChannel page = (*iter)[columns.type];
//	Glib::ustring name = (*iter)[columns.name];
//	std::string stream = (*iter)[columns.stream];
//
//	parent->record(stream,this);
//}
//
//void MMSChannel::store_selection()
//{
//	Glib::RefPtr < Gtk::TreeSelection > selection =
//	    this->get_selection();
//	Gtk::TreeModel::iterator iter = selection->get_selected();
//	if (!selection->count_selected_rows())
//		return ;
//	TypeChannel page = (*iter)[columns.type];
//	Glib::ustring name = (*iter)[columns.name];
//	std::string stream = (*iter)[columns.stream];
//
//	parent->getBookMarkChannel().saveLine(name,stream,page);
//
//}
//
