/*
 * =====================================================================================
 * 
 *       Filename:  ns_live_player.h
 * 
 *    Description:  nslive播放管理器
 * 
 *        Version:  1.0
 *        Created:  2007年12月03日 16时55分13秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  wind (xihe), xihels@gmail.com
 *        Company:  cyclone
 * 
 * =====================================================================================
 */

#ifndef _NS_LIVE_PLAYER_H_
#define _NS_LIVE_PLAYER_H_

#include "live_player.h"

class NsLivePlayer : public LivePlayer {
	public:
		NsLivePlayer(const std::string& id_);
		~NsLivePlayer();
		void play(GMplayer& gmplayer);
		void stop();
		const std::string& get_stream() { return id; }
	private:
		int ns_pid;
		std::string id;
};

#endif // _NS_LIVE_PLAYER_H_

