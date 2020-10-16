#include "File.h"

struct FileDir dir_read_BMP(struct FileDir Dir)
{
    DIR *pDir = NULL;//创建一个DIR*存储打开的路径
    struct dirent * pEnt = NULL;//创建一个存储路径中读到的信息

    int StrNum;
    char TYPE_BMP[] = ".bmp";
    char File_name[32*4];//用于存储并判断当前的文件路径是否是BMP
    //打开图片目录
    pDir = opendir("/mnt/udisk/bmp_1/");
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
                    sprintf(Dir.PhotoPath[Dir.filename],"/mnt/udisk/bmp_1/%s",pEnt->d_name);
                    Dir.filename++;
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
