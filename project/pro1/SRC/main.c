#include "main.h"



int main(int argc, char const *argv[])
{
//---------------------------------------------
    char *PhotoPath[256];//用于存储已经记录了图片路径的数组
    int filename;
    
    // while (1)
    // {
        DIR *pDir = NULL;//创建一个DIR*存储打开的路径
        struct dirent * pEnt = NULL;//创建一个存储路径中读到的信息
        char str[64];        //用于存储当前图片的图片路径
        //打开图片目录
        pDir = opendir("./Picture/");
        if (NULL == pDir)
        {
            perror("opendir");
            return -1;
        }
        //读取目录中的内容
        while (1)
        {
            
            pEnt = readdir(pDir);
            if(pEnt != NULL)
            {
                if (pEnt->d_type == DT_REG)//获得目录内的普通文件
                {
                    sprintf(str,"./Picture/%s",pEnt->d_name);
                    PhotoPath[filename] = str;
                    open_bmp(PhotoPath[filename]);
                    filename++;
                    usleep(300000);
                }
            }
            else
            {
                //filename = 0;
                closedir(pDir);
                break;
            }
        }
    //}
    
    
 //--------------------------------------------------  
 for (int i = 0; i < filename; i++)
 {
     printf(PhotoPath[i]);
     printf("\n");
 }
 
    printf("OKOK\n");
    return 0;
}
