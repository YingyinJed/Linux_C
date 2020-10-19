#include "ILI93xx.h"
/*
    函 数 名：LCDInit
    作    用：初始化打开LCD相关文件
    输入参数：无
    返    回：LCD结构体内含LCD文件编号
*/
struct Lcd_Init LCDInit(void)
{
    //声明初始化的结构体
    struct Lcd_Init LCD;
    // 1.  打开LCD设备文件
    LCD.fd_lcd = open( LCD_PATH , O_RDWR);
    if (-1 == LCD.fd_lcd)
    {
        perror("open LCD error");
        return LCD ;
    }

    //2、 内存映射
    LCD.p_lcd = (int *)mmap(NULL,	//需要内存映射的起始地址(通常设为 NULL, 代表让系统自动选定地址)
					LCD_SIZE,  //需要多大的一片内存(一字节为单位) LCD显示屏800*480个像素点 每个像素点占4字节
					PROT_READ | PROT_WRITE,  //映射区域可被读取 映射区域可被写入
					MAP_SHARED,					//对应射区域的写入数据会复制回文件内
					LCD.fd_lcd, 		//需要内存映射的文件的文件描述符
					0);			//参数offset为文件映射的偏移量, 通常设置为0
    if ( MAP_FAILED == LCD.p_lcd )
    {
        perror("mmap error");
        return LCD ;
    }
    return LCD ;
}
/*
    函数  名：open_bmp
    作    用：在LCD上显示特定图片
    输入参数：LCD结构体内含初始化中获得的LCD文件编号，
             需要显示的图片路径
    返    回：执行参数成功返回0，失败会返回-1
*/
int open_bmp(struct Lcd_Init LCD,char * PhotoPath)
{
    // 1.  打开图片
    int fd_bmp = open( PhotoPath , O_RDONLY);
    lseek(fd_bmp, 54, SEEK_SET);    //便宜54个字节跳过头信息
    if (-1 == fd_bmp)
    {
        perror("open BMP error");
        return -1 ;
    }
    // 2.  读取图像中的BGR颜色数据
    char  buf_bmp [ BMP_SIZE ] ;
    int ret_val = read(fd_bmp, buf_bmp , BMP_SIZE );


    // 3.  把图像中的BGR 数据转换为 ARGB  24位转为 32位
    int  buf_lcd [ H ][ W ] ; 
    int x, y ;
    for ( y = 0; y < H ; y++)
    {
        for ( x = 0; x < W ; x++)
        {
            buf_lcd[y][x] = buf_bmp[(x+y*800)*3 + 0] << 0 | 
                            buf_bmp[(x+y*800)*3 + 1] << 8 | 
                            buf_bmp[(x+y*800)*3 + 2] << 16 ;
        }
    }
    // 4.  把转换后的数据显示到LCD文件中//H=480 W=800
    for ( y = 0; y < H; y++)
    {
        for ( x = 0; x < W; x++)
        { // 解引用
           *(LCD.p_lcd+x+y*800) = buf_lcd[479-y][x] ;
        }
    }

    // 5.  关闭文件释放资源
    close(fd_bmp);

    
    return 0;
}
/*
    函数  名：open_bmp_left
    作    用：在LCD上显示特定图片，并有一个从左向右的动画
    输入参数：LCD结构体内含初始化中获得的LCD文件编号，
             需要显示的图片路径
    返    回：执行参数成功返回0，失败会返回-1
*/
int open_bmp_left(struct Lcd_Init LCD,char * PhotoPath)
{
    // 1.  打开图片
    int fd_bmp = open( PhotoPath , O_RDONLY);
    lseek(fd_bmp, 54, SEEK_SET);    //便宜54个字节跳过头信息
    if (-1 == fd_bmp)
    {
        perror("open BMP error");
        return -1 ;
    }
    // 2.  读取图像中的BGR颜色数据
    char  buf_bmp [ BMP_SIZE ] ;
    int ret_val = read(fd_bmp, buf_bmp , BMP_SIZE );

    // 3.  把图像中的BGR 数据转换为 ARGB  24位转为 32位
    int  buf_lcd [ H ][ W ] ; 
    int x, y ;
    for ( y = 0; y < H ; y++)
    {
        for ( x = 0; x < W ; x++)
        {
            buf_lcd[y][x] = buf_bmp[(x+y*800)*3 + 0] << 0 | 
                            buf_bmp[(x+y*800)*3 + 1] << 8 | 
                            buf_bmp[(x+y*800)*3 + 2] << 16 ;
        }
    }
    // 4.  把转换后的数据显示到LCD文件中//H=480 W=800
    //由于这个是左移，只考虑x轴因此只声明x轴
    int change_x = 0;
    while(1)
    {
        for ( y = 0; y < H; y++)
        {
            for ( x = 0; x < W; x++)
            { // 解引用
                if((x <= (4*(change_x+1)) && x >= (4 * change_x) ))
                {
                    *(LCD.p_lcd+x+y*800) = buf_lcd[479-y][x] ;
                }
            }
        }
        change_x++;
        if (change_x == 200)
        {
            break;
        }
    }
    // 5.  关闭文件释放资源
    close(fd_bmp);
    return 0;
}
/*
    函数  名：open_bmp_right
    作    用：在LCD上显示特定图片，并有一个从右向左的动画
    输入参数：LCD结构体内含初始化中获得的LCD文件编号，
             需要显示的图片路径
    返    回：执行参数成功返回0，失败会返回-1
*/
int open_bmp_right(struct Lcd_Init LCD,char * PhotoPath)
{
    // 1.  打开图片
    int fd_bmp = open( PhotoPath , O_RDONLY);
    lseek(fd_bmp, 54, SEEK_SET);    //便宜54个字节跳过头信息
    if (-1 == fd_bmp)
    {
        perror("open BMP error");
        return -1 ;
    }
    // 2.  读取图像中的BGR颜色数据
    char  buf_bmp [ BMP_SIZE ] ;
    int ret_val = read(fd_bmp, buf_bmp , BMP_SIZE );

    // 3.  把图像中的BGR 数据转换为 ARGB  24位转为 32位
    int  buf_lcd [ H ][ W ] ; 
    int x, y ;
    for ( y = 0; y < H ; y++)
    {
        for ( x = 0; x < W ; x++)
        {
            buf_lcd[y][x] = buf_bmp[(x+y*800)*3 + 0] << 0 | 
                            buf_bmp[(x+y*800)*3 + 1] << 8 | 
                            buf_bmp[(x+y*800)*3 + 2] << 16 ;
        }
    }
    // 4.  把转换后的数据显示到LCD文件中//H=480 W=800
    //由于这个是左移，只考虑x轴因此只声明x轴
    int change_x = 200;
    while(1)
    {
        for ( y = 0; y < H; y++)
        {
            for ( x = 0; x < W; x++)
            { // 解引用
                if((x <= (4*(change_x+1)) && x >= (4 * change_x) ))
                {
                    *(LCD.p_lcd+x+y*800) = buf_lcd[479-y][x] ;
                }
            }
        }
        change_x--;
        if (change_x < 0)
        {
            break;
        }
    }
    // 5.  关闭文件释放资源
    close(fd_bmp);
    return 0;
}
/*
    函数  名：open_bmp_up
    作    用：在LCD上显示特定图片，并有一个从上向下的动画
    输入参数：LCD结构体内含初始化中获得的LCD文件编号，
             需要显示的图片路径
    返    回：执行参数成功返回0，失败会返回-1
*/
int open_bmp_up(struct Lcd_Init LCD,char * PhotoPath)
{
    // 1.  打开图片
    int fd_bmp = open( PhotoPath , O_RDONLY);
    lseek(fd_bmp, 54, SEEK_SET);    //便宜54个字节跳过头信息
    if (-1 == fd_bmp)
    {
        perror("open BMP error");
        return -1 ;
    }
    // 2.  读取图像中的BGR颜色数据
    char  buf_bmp [ BMP_SIZE ] ;
    int ret_val = read(fd_bmp, buf_bmp , BMP_SIZE );

    // 3.  把图像中的BGR 数据转换为 ARGB  24位转为 32位
    int  buf_lcd [ H ][ W ] ; 
    int x, y ;
    for ( y = 0; y < H ; y++)
    {
        for ( x = 0; x < W ; x++)
        {
            buf_lcd[y][x] = buf_bmp[(x+y*800)*3 + 0] << 0 | 
                            buf_bmp[(x+y*800)*3 + 1] << 8 | 
                            buf_bmp[(x+y*800)*3 + 2] << 16 ;
        }
    }
    // 4.  把转换后的数据显示到LCD文件中//H=480 W=800
    //由于这个是左移，只考虑x轴因此只声明x轴
    int change_y = 0;
    while(1)
    {
        for ( y = 0; y < H; y++)
        {
            for ( x = 0; x < W; x++)
            { // 解引用
                if((y <= (2*(change_y+1)) && y >= (2 * change_y) ))
                {
                    *(LCD.p_lcd+x+y*800) = buf_lcd[479-y][x] ;
                }
            }
        }
        change_y++;
        if (change_y > 240)
        {
            break;
        }
    }
    // 5.  关闭文件释放资源
    close(fd_bmp);
    return 0;
}
/*
    函数  名：open_bmp_down
    作    用：在LCD上显示特定图片，并有一个从下向上的动画
    输入参数：LCD结构体内含初始化中获得的LCD文件编号，
             需要显示的图片路径
    返    回：执行参数成功返回0，失败会返回-1
*/
int open_bmp_down(struct Lcd_Init LCD,char * PhotoPath)
{
    // 1.  打开图片
    int fd_bmp = open( PhotoPath , O_RDONLY);
    lseek(fd_bmp, 54, SEEK_SET);    //便宜54个字节跳过头信息
    if (-1 == fd_bmp)
    {
        perror("open BMP error");
        return -1 ;
    }
    // 2.  读取图像中的BGR颜色数据
    char  buf_bmp [ BMP_SIZE ] ;
    int ret_val = read(fd_bmp, buf_bmp , BMP_SIZE );

    // 3.  把图像中的BGR 数据转换为 ARGB  24位转为 32位
    int  buf_lcd [ H ][ W ] ; 
    int x, y ;
    for ( y = 0; y < H ; y++)
    {
        for ( x = 0; x < W ; x++)
        {
            buf_lcd[y][x] = buf_bmp[(x+y*800)*3 + 0] << 0 | 
                            buf_bmp[(x+y*800)*3 + 1] << 8 | 
                            buf_bmp[(x+y*800)*3 + 2] << 16 ;
        }
    }
    // 4.  把转换后的数据显示到LCD文件中//H=480 W=800
    //由于这个是左移，只考虑x轴因此只声明x轴
    int change_y = 240;
    while(1)
    {
        for ( y = 0; y < H; y++)
        {
            for ( x = 0; x < W; x++)
            { // 解引用
                if((y <= (2*(change_y+1)) && y >= (2 * change_y) ))
                {
                    *(LCD.p_lcd+x+y*800) = buf_lcd[479-y][x] ;
                }
            }
        }
        change_y--;
        if (change_y < 0)
        {
            break;
        }
    }
    // 5.  关闭文件释放资源
    close(fd_bmp);
    return 0;
}

int open_bmp_X_Y(struct Lcd_Init LCD,char * PhotoPath,int Touch_x,int Touch_y)
{
    // 1.  打开图片
    int fd_bmp = open( PhotoPath , O_RDONLY);
    lseek(fd_bmp, 54, SEEK_SET);    //便宜54个字节跳过头信息
    if (-1 == fd_bmp)
    {
        perror("open BMP error");
        return -1 ;
    }
    // 2.  读取图像中的BGR颜色数据
    char  buf_bmp [ BMP_SIZE ] ;
    int ret_val = read(fd_bmp, buf_bmp , BMP_SIZE );

    // 3.  把图像中的BGR 数据转换为 ARGB  24位转为 32位
    int  buf_lcd [ H ][ W ] ; 
    int x, y ;
    for ( y = 0; y < H ; y++)
    {
        for ( x = 0; x < W ; x++)
        {
            buf_lcd[y][x] = buf_bmp[(x+y*800)*3 + 0] << 0 | 
                            buf_bmp[(x+y*800)*3 + 1] << 8 | 
                            buf_bmp[(x+y*800)*3 + 2] << 16 ;
        }
    }
    // 4.  把转换后的数据显示到LCD文件中//H=480 W=800
        for ( y = 0; y < H; y++)
        {
            for ( x = 0; x < W; x++)
            { // 解引用
                if(((y <= (Touch_y + 25) && y >= (Touch_y - 25))) && ((x <= (Touch_x + 25) && x >= (Touch_x - 25))))
                {
                    *(LCD.p_lcd+x+y*800) = buf_lcd[479-y][x] ;
                }
            }
        }
    // 5.  关闭文件释放资源
    close(fd_bmp);
    return 0;
}

void Display_pur(struct Lcd_Init LCD,int color)
{
    // 4.  把转换后的数据显示到LCD文件中//H=480 W=800
        for (int y = 0; y < H; y++)
        {
            for (int x = 0; x < W; x++)
            { // 解引用
                *(LCD.p_lcd+x+y*800) = color;
            }
        }
}

/*
    函数  名：lcd_exit
    作    用：关闭LCD相关文件
    输入参数：需要关闭的LCD结构体
    返    回：无
*/
void lcd_exit(struct Lcd_Init LCD)
{
    close(LCD.fd_lcd);//关闭文件
    munmap(LCD.p_lcd , LCD_SIZE);//释放内存
}
