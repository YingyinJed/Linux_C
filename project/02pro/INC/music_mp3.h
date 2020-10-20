#ifndef _music_mp3_H_
#define _music_mp3_H_

#include "main.h"

#define MPLAYER_TIPS1   "mplayer -quiet -slave -input file="
#define FIFOPATH_MP3    "/tmp/MP3"
#define MPLAYER_INIT_V  " -af volume=-30 "

void MP3_Init(int fd_fifo,char * MP3path);
void MP3_Q_P(int fd_fifo,char * MP3path);
#endif
