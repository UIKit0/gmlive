#ifndef GMLIVE_H_
#define GMLIVE_H_

#define PAGE_PICTURE 0
#define PAGE_MPLAYER 1
#define NSLIVESTREAM "http://127.0.0.1:9000"
#define SOPCASTSTREAM "http://localhost:8908/tv.asf"

enum TypeChannel{
	NSLIVE_CHANNEL,
	MMS_CHANNEL,
	SOPCAST_CHANNEL,
	GROUP_CHANNEL,
	NONE
};


#endif