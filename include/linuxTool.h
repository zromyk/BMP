/*
 * @Description: 
 * @FilePath: /BMP/include/linuxTool.h
 * @Author: Wei Zhou
 * @Github: https://github.com/muyi2414
 * @Date: 2020-03-08 16:14:42
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2020-03-10 00:31:11
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>

class FullPath
{
public:
	/**
	 * @description: 构造函数
	 * @param {type} argv0 {char const *}: 请设置为arg[0]
	 * 				 fileName {const char *}: 文件名
	 * @return: None
	 */
	FullPath(char const *argv0, const char *fileName);
	~FullPath()
	{
		delete[] fullPath;
	};
	/**
	 * @description: 构造函数
	 * @param {type} None
	 * @return: fullPath {const char*}: 文件的完整路径
	 */
	const char* getFullPath()
	{
		return fullPath;
	};
private:
	char* fullPath;
};