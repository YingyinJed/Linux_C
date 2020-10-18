#ifndef _ILI93XX_H_
#define _ILI93XX_H_

#include <main.h>

/*-------------------宏定义-----------------*/
#define    LCD_PATH         "/dev/fb0"
#define    W                800
#define    H                480
#define    LCD_SIZE         W*H*4                
#define    BMP_SIZE         W*H*3 
/*-------------------定义结构体---------------*/
struct Lcd_Init
{
    int fd_lcd;
    int* p_lcd;
};

/*-------------------定义函数-----------------*/
struct Lcd_Init LCDInit(void);  //初始化LCD

int open_bmp(struct Lcd_Init LCD,char * PhotoPath); //在LCD中绘图
int open_bmp_left(struct Lcd_Init LCD,char * PhotoPath);//在LCD上从左向右的显示图片
int open_bmp_right(struct Lcd_Init LCD,char * PhotoPath);//在LCD上从右向左的显示图片
int open_bmp_up(struct Lcd_Init LCD,char * PhotoPath);//在LCD上从上至下的显示图片
int open_bmp_down(struct Lcd_Init LCD,char * PhotoPath);//在LCD上从下至上的显示图片

void lcd_exit(struct Lcd_Init LCD);

#endif
