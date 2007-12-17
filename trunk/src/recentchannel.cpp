/*
 * =====================================================================================
 *
 *       Filename:  recentchannel.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2007年12月02日 20时19分58秒 CST
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
#include <vector>
#include "recentchannel.h"
#include "MainWindow.h"
#include "mmsLivePlayer.h"
#include "sopcastLivePlayer.h"
#include "nsLivePlayer.h"

RecentChannel::RecentChannel(MainWindow* parent_):Channel( parent_)
{
	init();
}

RecentChannel::~RecentChannel()
{}

LivePlayer* RecentChannel::get_player( const std::string& stream,TypeChannel page)
{
	switch(page) {
		case MMS_CHANNEL:
			return new MmsLivePlayer(stream);
		case NSLIVE_CHANNEL:
			{
				return new NsLivePlayer(stream);
			}
		case SOPCAST_CHANNEL:
			return new SopcastLivePlayer(stream);
	}
}
void RecentChannel::init()
{
	char buf[512];
	char* homedir = getenv("HOME");
	snprintf(buf, 512,"%s/.gmlive/recent.lst",homedir);
	std::ifstream file(buf);
	if(!file){
		std::ofstream out(buf);
		out.close();
		file.open(buf);
		printf("buf is %s\n",buf);
		std::cout<<"file error\n";
		//return;
	}
	std::string line;
	std::string last;
	std::string name;
	std::string stream;
	std::string type;
	if(file){
		while(std::getline(file,line)){
			size_t pos = line.find_first_of("#");
			if(pos==std::string::npos)
				continue;
			name = line.substr(0,pos);
			last = line.substr(pos+1,std::string::npos);

			pos = last.find_first_of("#");
			if(pos==std::string::npos)
				continue;
			stream=last.substr(0,pos);
			type= last.substr(pos+1,std::string::npos);
			
			int id=0;
			addLine(id,name,stream,type);
		}
	}

	file.close();

}

void  RecentChannel::addLine(const int num,const Glib::ustring& name,const std::string& stream_,const Glib::ustring& type)
{
	Gtk::TreeModel::iterator iter = m_liststore->prepend();
	(*iter)[columns.name] = name;
	(*iter)[columns.freq] = 100;
	(*iter)[columns.stream]=stream_;
	/*
	if(0 == num)
		(*iter)[columns.type]=MMS_CHANNEL;
	else
		(*iter)[columns.type]=NSLIVE_CHANNEL;
		*/
	TypeChannel type_;
	if("mms"==type)
		type_ = MMS_CHANNEL;
	else if("nslive" == type)
		type_ = NSLIVE_CHANNEL;
	else if("sopcast" == type)
		type_ = SOPCAST_CHANNEL;
	else
		type_ = NONE;
	(*iter)[columns.type]=type_ ;

}

void RecentChannel::saveLine(const Glib::ustring & name,const std::string& stream_,TypeChannel type)
{

	char buf[512];
	char* homedir = getenv("HOME");
	snprintf(buf, 512,"%s/.gmlive/recent.lst",homedir);
	std::ifstream file(buf);
	std::vector<std::string> list;
	if(!file){
		printf("buf is %s\n",buf);
		std::cout<<"file error\n";
		return;
	}
	std::string line;
	int num=0;
	if(file){
		while(std::getline(file,line)){
			list.push_back(line);
			num++;
			if(10==num)
				break;
		}
	}
	file.close();
	std::string stream;
	
	std::string strtype;
	if(type == MMS_CHANNEL)
	{
		stream = name +"\t#"+stream_+"\t#mms";
		strtype = "mms";
	}
	else if (type == SOPCAST_CHANNEL)
	{
		stream = name +"\t#"+stream_+"\t#sopcast";
		strtype = "sopcast";
	}
	else
	{
		stream = name +"\t#"+stream_+"\t#nslive";
		strtype = "nslive";
	}
	std::vector<std::string>::iterator iter = std::find(list.begin(),list.end(),stream);
	if(iter == list.end())
	{
		list.push_back(stream);
		std::ofstream outfile(buf);
		for(iter=list.begin();iter!=list.end();++iter)
		{
			if(iter == list.begin()&&(num==10))
				;
			else
				outfile<<*iter<<std::endl;
		}
		outfile.close();

		int users  =0;
		addLine(users, name,stream_,strtype);
	}

}

