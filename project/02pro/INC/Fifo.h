#ifndef _FIFO_H_
#define _FIFO_H_

#include "main.h"

int Fifo_Init(char* fifopath);
int Mk_Fifo(int fd_fifo,char * command);

#define FIFOPATH_MP3    "/tmp/MP3"

#endif