/*
 * @Description: 
 * @FilePath: /BMP/src/linuxTool.cpp
 * @Author: Wei Zhou
 * @Github: https://github.com/muyi2414
 * @Date: 2020-03-08 16:06:30
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2020-03-10 00:31:22
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "linuxTool.h"

/**
 * @description: 构造函数
 * @param {type} argv0 {char const *}: 请设置为arg[0]
 * 				 fileName {const char *}: 文件名
 * @return: None
 */
FullPath::FullPath(char const *argv0, const char *fileName)
{
    // 动态分配文件路径存储字符串
    fullPath = new char[strlen(argv0) + strlen(fileName)];
    char *pStr = fullPath + strlen(argv0);
    strcpy(fullPath, argv0);

    while(pStr > fullPath)
    {
        if(*(--pStr) == '/') // 去掉最后面的可执行程序程序路径中的程序名称
        {
            *(pStr+1) = 0x00;
            strcat(fullPath, fileName);
            break;
        }
    }
}
