#ifndef _FIFO_H_
#define _FIFO_H_

#include "main.h"

int Fifo_Init(char* fifopath);
int WR_Fifo(int fd_fifo,char * command);


#endif