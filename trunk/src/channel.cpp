/*
 * =====================================================================================
 *
 *       Filename:  channel.cpp
 *
 *    Description:  频道列表
 *
 *        Version:  1.0
 *        Created:  2007年12月01日 19时25分31秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */


#include <sstream>
#include "channel.h"
#include "MainWindow.h"
#include "livePlayer.h"
#include "recentchannel.h"
#include "bookmarkchannel.h"
#include "ChannelsTooltips.h"
#include <cassert>
#include <glib/gi18n.h>

Channel::Channel(MainWindow* parent_):parent( parent_)
{
	set_flags(Gtk::CAN_FOCUS);
	set_rules_hint(false);

	tooltips = new ChannelsTooltips(this);

	m_liststore = Gtk::TreeStore::create(columns);
	Glib::RefPtr<Gtk::TreeModelFilter> filter = 
		Gtk::TreeModelFilter::create(m_liststore);
	//filter->set_visible_column(1);
	set_model(filter);

	append_column(_("channels"), columns.name);
	append_column(_("bitrate"), columns.freq);
	append_column(_("user"), columns.users);

	/*
	set_has_tooltip();
	set_tooltip_window(*tooltips);
	signal_query_tooltip().connect(sigc::mem_fun(*this,
				&Channel::on_tooltip_show));
	*/
	signal_motion_notify_event().
		connect(sigc::mem_fun(*this, &Channel::on_motion_event),
				false);
	signal_leave_notify_event().
		connect(sigc::mem_fun(*this, &Channel::on_leave_event),
				false);

	filter->set_visible_func(sigc::mem_fun(*this, &Channel::on_visible_func));

	show();
}

Channel::~Channel()
{
}


Gtk::TreeModel::iterator Channel::getListIter(Gtk::TreeModel::
		Children children, const std::string& groupname)
{
	return find_if(children.begin(),
			children.end(),
			bind2nd(CompareChannel(columns),groupname));
}
Gtk::TreeModel::iterator Channel::addGroup(const Glib::ustring& group)
{
	Gtk::TreeModel::iterator iter = m_liststore->append();
	(*iter)[columns.name] = group;
	(*iter)[columns.type]=GROUP_CHANNEL;

	return iter;
}

bool Channel::on_button_press_event(GdkEventButton * ev)
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
		return false;

	if ((ev->type == GDK_2BUTTON_PRESS ||
				ev->type == GDK_3BUTTON_PRESS) && ev->button != 3) {
		if(GROUP_CHANNEL != (*iter)[columns.type]){
			parent->get_gmplayer()->set_record(false);
			play_selection_iter(iter);
		}
		else {
			if(this->row_expanded(path))
				this->collapse_row(path);
			else{
				this->expand_row(path,false);
				this->scroll_to_row(path);
			}
		}
	} else if ((ev->type == GDK_BUTTON_PRESS)
			&& (ev->button == 3)) {
		if(GROUP_CHANNEL == (*iter)[columns.type])
			return false;
		Gtk::Menu* pop_menu = 
			parent->get_channels_pop_menu();
		if (pop_menu)
			pop_menu->popup(ev->button, ev->time);
		return true;
	}
	return false;
}

void Channel::play_selection()
{
	Glib::RefPtr < Gtk::TreeSelection > selection =
		this->get_selection();
	Gtk::TreeModel::iterator iter = selection->get_selected();
	if (!selection->count_selected_rows()) {
		return ;
	}
	
	if(GROUP_CHANNEL != (*iter)[columns.type]) {
		parent->get_gmplayer()->set_record(false);
		play_selection_iter(iter);
	}

}


void Channel::record_selection()
{
	Glib::RefPtr < Gtk::TreeSelection > selection =
		this->get_selection();
	Gtk::TreeModel::iterator iter = selection->get_selected();
	if (!selection->count_selected_rows()) {
		return ;
	}

	if(GROUP_CHANNEL != (*iter)[columns.type]) {
		Gtk::FileChooserDialog dlg(*parent,
				_("Choose File"), 
				Gtk::FILE_CHOOSER_ACTION_SAVE);
		dlg.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dlg.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
		if (Gtk::RESPONSE_OK == dlg.run()) {
			Glib::ustring outfilename = dlg.get_filename();
			if (outfilename.empty())
				return;
			GMplayer* player = parent->get_gmplayer();
			if (player->recording())
				return;
			player->set_record(true);
			const Glib::ustring& name= (*iter)[columns.name];
			player->set_outfilename(outfilename, name);
			play_selection_iter(iter);
		}
	}
}

std::string Channel::get_stream()
{
	Glib::RefPtr < Gtk::TreeSelection > selection =
		this->get_selection();
	Gtk::TreeModel::iterator iter = selection->get_selected();
	if (!selection->count_selected_rows())
		return std::string();
	return (*iter)[columns.stream];
}

void Channel::store_selection()
{
	Glib::RefPtr < Gtk::TreeSelection > selection =
		this->get_selection();
	Gtk::TreeModel::iterator iter = selection->get_selected();
	if (!selection->count_selected_rows())
		return ;
	TypeChannel page = (*iter)[columns.type];
	Glib::ustring name = (*iter)[columns.name];
	std::string stream = (*iter)[columns.stream];

	BookMarkChannel* bc =
		dynamic_cast<BookMarkChannel*>(parent->get_bookmark_channel());
	if (this != bc)
		bc->saveLine(name,stream,page);

	RecentChannel* rc =
		dynamic_cast<RecentChannel*>(parent->get_recent_channel());
	if (this != rc)
		rc->saveLine(name,stream,page);
}


void Channel::play_selection_iter(Gtk::TreeModel::iterator& iter)
{
	//printf("\nplay_selection_iter\n");
	TypeChannel page = (*iter)[columns.type];
	Glib::ustring name = (*iter)[columns.name];
	std::string stream = (*iter)[columns.stream];

	if(GROUP_CHANNEL == (*iter)[columns.type]) { // 点在分组名上, 什么也不做
		return;
	}

	LivePlayer* lp = parent->get_live_player();
	LivePlayer* live_player = get_player(stream, page);

	parent->set_live_player(live_player, name);
	RecentChannel* rc =
		dynamic_cast<RecentChannel*>(parent->get_recent_channel());
	if (this != rc)
		rc->saveLine(name,stream,page);
}

void Channel::play_stream(const std::string& stream_url, TypeChannel stream_type,const std::string& name)
{

	LivePlayer* lp = parent->get_live_player();
	LivePlayer* live_player = get_player(stream_url, stream_type);

	parent->set_live_player(live_player, name);
	//RecentChannel* rc =
	//	dynamic_cast<RecentChannel*>(parent->get_recent_channel());
	//if (this != rc)
	//	rc->saveLine(name,stream,page);


}

void Channel::search_channel(const Glib::ustring& name_)
{
	search_channel_name = name_;
	Glib::RefPtr<Gtk::TreeModelFilter> filter = 
		Glib::RefPtr<Gtk::TreeModelFilter>::cast_dynamic(get_model());
	filter->refilter();
	if (!name_.empty())
		expand_all();
}


bool Channel::on_visible_func(const Gtk::TreeModel::iterator& iter)
{
	if ((*iter)[columns.type] == GROUP_CHANNEL) {
		return true;
	}
	const Glib::ustring& name = (*iter)[columns.name];
	return name.find(search_channel_name, 0) != Glib::ustring::npos;
}

bool Channel::on_leave_event(GdkEventCrossing * ev)
{
	if (tipTimeout.connected()) {
		tipTimeout.disconnect();
	}
	return false;
}

bool Channel::tooltip_timeout(GdkEventMotion * ev)
{
	Gtk::TreeModel::Path path;
	Gtk::TreeViewColumn * column;
	int cell_x, cell_y;
	if (this->
			get_path_at_pos((int) ev->x, (int) ev->y, path, column, cell_x,
				cell_y)) {
		Gtk::TreeModel::iterator iter =
			this->get_model()->get_iter(path);
		if (!iter)
			return false;
		TypeChannel type = (*iter)[columns.type];
		Glib::ustring type_;
		if(PPLIVE_CHANNEL == type)
			type_  = _("PPLive Stream");
		else if(SOPCAST_CHANNEL == type)
			type_ = _("SopCast Stream");
		else if(MMS_CHANNEL == type)
			type_ = _("MMS stream");
		else if(PPS_CHANNEL == type)
			type_ = _("PPS stream");
		Glib::ustring name = (*iter)[columns.name];
		int num = (*iter)[columns.users];
		std::stringstream ss;
		ss<<num;
		std::string user=ss.str();
		std::string stream = (*iter)[columns.stream];
		Glib::ustring text;

		if(PPLIVE_CHANNEL == type)
			text = "<span weight='bold'>" +name +"\n" + _("users:")+user+
				"\n<span weight='bold'></span>"+_("Type:")+type_+"\n</span>";
		else
			text = "<span weight='bold'>" +name +"\n" + _("users:")+user+
				"\nURL:</span> " + stream +"\n<span weight='bold'>"+_("Type:")+type_+"\n</span>";
		//Glib::RefPtr<Gdk::Pixbuf> logo= Gdk::Pixbuf::create_from_file(DATA_DIR"/gmlive.png");

		//tooltips->setImage(logo);
		tooltips->setLabel(text);
		tooltips->showTooltip(ev);

	}
	return false;
}
bool Channel::on_motion_event(GdkEventMotion * ev)
{
	Gtk::TreeModel::Path path;
	Gtk::TreeViewColumn * column;
	int cell_x, cell_y;
	int delay = 600;

	if (tipTimeout.connected()) {

		tipTimeout.disconnect();
		tooltips->hideTooltip();
	}
	if (this->
			get_path_at_pos((int) ev->x, (int) ev->y, path, column, cell_x,
				cell_y)) {
		Gtk::TreeModel::iterator iter =
			this->get_model()->get_iter(path);
		TypeChannel type = (*iter)[columns.type];
		if (GROUP_CHANNEL != type)
			tipTimeout =
				Glib::signal_timeout().connect(sigc::bind <
						GdkEventMotion *
						>(sigc::
							mem_fun(*this,
								&Channel::
								tooltip_timeout),
							ev), delay);
		else
			tooltips->hideTooltip();
	} else
		tooltips->hideTooltip();

	return true;
}


bool Channel::on_tooltip_show(int x, int y, bool key_mode, const Glib::RefPtr<Gtk::Tooltip>& tooltip)
{
	Gtk::TreeModel::Path path;
	Gtk::TreeViewColumn * column;
	int cell_x, cell_y;
	if (this->
			get_path_at_pos(x, y, path, column, cell_x,
				cell_y)) {
		Gtk::TreeModel::iterator iter =
			this->get_model()->get_iter(path);
		if (!iter){
			return false;
		}
		TypeChannel type = (*iter)[columns.type];
		Glib::ustring type_;
		if(PPLIVE_CHANNEL == type)
			type_  = _("PPLive Stream");
		else if(SOPCAST_CHANNEL == type)
			type_ = _("SopCast Stream");
		else if(MMS_CHANNEL == type)
			type_ = _("MMS stream");
		Glib::ustring name = (*iter)[columns.name];
		int num = (*iter)[columns.users];
		std::stringstream ss;
		ss<<num;
		std::string user=ss.str();
		std::string stream = (*iter)[columns.stream];
		Glib::ustring text;

		if(PPLIVE_CHANNEL == type){
			text = "<span weight='bold'>" +name +"\n" + _("users:")+user+
				"\n<span weight='bold'></span>"+_("Type:")+type_+"\n</span>";

		}
		else
			text = "<span weight='bold'>" +name +"\n" + _("users:")+user+
				"\nURL:</span> " + stream +"\n<span weight='bold'>"+_("Type:")+type_+"\n</span>";
		//Glib::RefPtr<Gdk::Pixbuf> logo= Gdk::Pixbuf::create_from_file(DATA_DIR"/gmlive.png");

		//tooltips->setImage(logo);
		tooltips->setLabel(text);
		//tooltips->showTooltip(ev);
		return true;

	}
	return false;
}

Glib::ustring Channel::user_select_list(const char* title)
{
	//属于读取默认列表不正确的处理
	std::string filename;
	Gtk::FileChooserDialog dialog(_("Please select a channel list file"),Gtk::FILE_CHOOSER_ACTION_OPEN);
	Gtk::MessageDialog askDialog(_("open channles error")
			,false
			,Gtk::MESSAGE_QUESTION
			,Gtk::BUTTONS_OK_CANCEL
			);
	askDialog.set_secondary_text(title);
	if (askDialog.run() == Gtk::RESPONSE_OK) {
		//open a file select window
		dialog.add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::OPEN,Gtk::RESPONSE_OK);
		if ( dialog.run() == Gtk::RESPONSE_OK)
			return dialog.get_filename();

	}
	return "";
}

