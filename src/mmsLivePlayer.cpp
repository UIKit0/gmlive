/*
 * =====================================================================================
 *
 *       Filename:  mmsLivePlayer.cpp
 *
 *    Description:  mms播放管理器 
 *
 *        Version:  1.0
 *        Created:  2007年12月03日 16时25分41秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wind (xihe), xihels@gmail.com
 *        Company:  cyclone
 *
 * =====================================================================================
 */

#include "mmsLivePlayer.h"
#include <gmplayer.h>
#include <gmlive.h>

MmsLivePlayer::MmsLivePlayer(const std::string& fname) :
	filename(fname)
{
}

MmsLivePlayer::~MmsLivePlayer()
{
	//gmp.stop();
	printf("mms exit\n");
}
void MmsLivePlayer::start(GMplayer& gmp)
{
	std::string& cache = GMConf["mms_mplayer_cache"];
	int icache = atoi(cache.c_str());
	icache = icache > 64 ? icache : 8192;

	gmp.set_cache(icache);
	gmp.start(filename);
}

void MmsLivePlayer::stop()
{
	//gmp.stop();
}

