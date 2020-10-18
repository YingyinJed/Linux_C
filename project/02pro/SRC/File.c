#include "File.h"

struct FileDir dir_read(struct FileDir Dir,char * url)
{
    DIR *pDir = NULL;//创建一个DIR*存储打开的路径
    struct dirent * pEnt = NULL;//创建一个存储路径中读到的信息
    /*---------定义需要寻找的特定文件---------------*/
    //寻找路径下的bmp文件
    char TYPE_BMP[] = ".bmp";
    char background[] = "background";
    char music_play[] = "music_play";
    char music_stop[] = "music_stop";
    //寻找路径下的mp3文件
    char TYPE_MP3[] = ".mp3";

    char File_name[32*4];//用于存储并判断当前的文件路径是否是BMP
    //打开图片目录
    pDir = opendir(url);
    if (NULL == pDir)
    {
        perror("opendir");
        return Dir;
    }
    //读取目录中的内容
    while (1)
    {
        pEnt = readdir(pDir);
        if(pEnt != NULL)
        {
            sprintf(File_name,"%s",pEnt->d_name);
            if (pEnt->d_type == DT_REG)//获得目录内的普通文件
            {   
                if (strstr(File_name,TYPE_BMP) != NULL)//判断是不是bmp文件
                {
                    //如果这幅图是背景图，则存在最后一个
                    if (strstr(File_name,background) != NULL)
                    {
                        //先获得路径
                        sprintf(Dir.PhotoPath[BackGround_NUM],url);
                        //追加路径文件名
                        strcat(Dir.PhotoPath[BackGround_NUM],pEnt->d_name);
                    }//如果这幅图是音乐播放图则放在倒数第二个
                    else if (strstr(File_name,music_play) != NULL)
                    {
                        //先获得路径
                        sprintf(Dir.PhotoPath[Music_Play],url);
                        //追加路径文件名
                        strcat(Dir.PhotoPath[Music_Play],pEnt->d_name);
                    }
                    //如果这副图是音乐暂停图则放在倒数第三个
                    else if (strstr(File_name,music_stop) != NULL)
                    {
                        //先获得路径
                        sprintf(Dir.PhotoPath[Music_Stop],url);
                        //追加路径文件名
                        strcat(Dir.PhotoPath[Music_Stop],pEnt->d_name);
                    }
                    //如果都不是说明是普通图片则按顺序存储即可
                    else
                    {
                        //先获得路径
                        sprintf(Dir.PhotoPath[Dir.fileNum],url);
                        //追加路径文件名
                        strcat(Dir.PhotoPath[Dir.fileNum],pEnt->d_name);
                        printf("%s\n",Dir.PhotoPath[Dir.fileNum]);
                        Dir.fileNum++;
                        if (Dir.fileNum >= Music_Stop)
                        {
                            printf("Exceeds the number of files that can be stored\n");
                            printf("There are %d photos",Dir.fileNum);
                        }
                    }  
                }
                if (strstr(File_name,TYPE_MP3) != NULL)//判断是不是mp3文件
                {
                    //先获得路径
                    sprintf(Dir.PhotoPath[Dir.fileNum],url);
                    //追加路径文件名
                    strcat(Dir.PhotoPath[Dir.fileNum],pEnt->d_name);
                    printf("%s\n",Dir.PhotoPath[Dir.fileNum]);
                    Dir.fileNum++;
                }
                
            }
        }
        else
        {
            closedir(pDir);//读取完成关闭目录
            break;
        }
    }
    return Dir;
}
