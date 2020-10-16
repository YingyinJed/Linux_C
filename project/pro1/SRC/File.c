#include "File.h"

struct FileDir dir_read(struct FileDir Dir)
{
    DIR *pDir = NULL;//创建一个DIR*存储打开的路径
    struct dirent * pEnt = NULL;//创建一个存储路径中读到的信息
    //打开图片目录
    pDir = opendir("./Picture/");
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
            if (pEnt->d_type == DT_REG)//获得目录内的普通文件
            {
                sprintf(Dir.PhotoPath[Dir.filename],"./Picture/%s",pEnt->d_name);
                Dir.filename++;
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
