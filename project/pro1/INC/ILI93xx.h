#ifndef _ILI93XX_H_
#define _ILI93XX_H_

#include <main.h>

#define    TOUCH_PATH  "/dev/input/event0"
#define    LCD_PATH         "/dev/fb0"
#define    W                800
#define    H                480
#define    LCD_SIZE         W*H*4                
#define    BMP_SIZE         W*H*3 

int open_bmp(char * PhotoPath);

#endif