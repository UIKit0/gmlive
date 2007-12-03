/*
 * =====================================================================================
 *
 *       Filename:  nslivechannel.cpp
 *
 *    Description:  support nslive channel list
 *
 *        Version:  1.0
 *        Created:  2007年12月02日 09时00分39秒 CST
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
#include "nslivechannel.h"
#include "MainWindow.h"


NSLiveChannel::NSLiveChannel(MainWindow* parent_):parent( parent_)
{


}
void NSLiveChannel::init()
{

	char buf[512];
	char* homedir = getenv("HOME");
	snprintf(buf, 512,"%s/.gmlive/nslive.lst",homedir);
	std::ifstream file(buf);
	if(!file){
		printf("buf is %s\n",buf);
		std::cout<<"file error\n";
		return;
	}
	std::string line;
	std::string name;
	std::string uid;
	std::string stream(NSLIVESTREAM);
	std::string groupname;
	int id;
	if(file){
		while(std::getline(file,line)){
			size_t pos = line.find_first_of("#");
			if(pos==std::string::npos)
				continue;
			name = line.substr(0,pos);
			size_t pos2 = line.find_last_of(";");
			if(pos2==std::string::npos)
				continue;
			groupname= line.substr(pos2+1,std::string::npos);
			uid = line.substr(pos+1,pos2);
			id = atoi(uid.c_str());
			addLine(id,name,stream,groupname);
		}
	}

	file.close();

}


void NSLiveChannel::addLine(const int num, const Glib::ustring & name,const std::string& stream,const Glib::ustring& groupname)
{
	Gtk::TreeModel::Children children = m_liststore->children();
	Gtk::TreeModel::iterator listiter;
	listiter = getListIter(children,groupname);
	if(listiter == children.end())
		listiter = addGroup(groupname);

	Gtk::TreeModel::iterator iter = m_liststore->append(listiter->children());
	//Gtk::TreeModel::iterator iter = m_liststore->append();
	(*iter)[columns.id] = num;
	(*iter)[columns.name] = name;
	(*iter)[columns.freq] = 350;
	(*iter)[columns.stream]=stream;
	(*iter)[columns.type]=NSLIVE_CHANNEL;

}

/*
Gtk::TreeModel::iterator NSLiveChannel::addGroup(const Glib::ustring& group)
{
	Gtk::TreeModel::iterator iter = m_liststore->append();
	(*iter)[columns.name] = group;
	(*iter)[columns.type]=GROUP_CHANNEL;

	return iter;
}
*/

bool NSLiveChannel::on_button_press_event(GdkEventButton * ev)
{
	bool result = Gtk::TreeView::on_button_press_event(ev);

	Glib::RefPtr < Gtk::TreeSelection > selection =
	    this->get_selection();
	Gtk::TreeModel::iterator iter = selection->get_selected();
	if (!selection->count_selected_rows())
		return result;

	Gtk::TreeModel::Path path(iter);
	Gtk::TreeViewColumn * tvc;
	int cx, cy;
					/** get_path_at_pos() 是为确认鼠标是否在选择行上点击的*/
	if (!this->
	    get_path_at_pos((int) ev->x, (int) ev->y, path, tvc, cx, cy))
		return FALSE;
	if ((ev->type == GDK_2BUTTON_PRESS ||
	     ev->type == GDK_3BUTTON_PRESS)) {
		if(NSLIVE_CHANNEL == (*iter)[columns.type]){
			int channle_num = (*iter)[columns.id];
			Glib::ustring name = (*iter)[columns.name];
			std::string stream = (*iter)[columns.stream];
			parent->nslive_play(channle_num);
			parent->getRecentChannel().saveLine(channle_num,name,stream);
		}
		else if(GROUP_CHANNEL == (*iter)[columns.type]){
			if(this->row_expanded(path))
				this->collapse_row(path);
			else{
				this->expand_row(path,false);
				this->scroll_to_row(path);
			}
		}


	} else if ((ev->type == GDK_BUTTON_PRESS)
		   && (ev->button == 3)) {
	}

}
