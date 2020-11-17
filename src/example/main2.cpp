/*
 * @Description: 
 * @FilePath: /ImageDrive/main.cpp
 * @Author: Wei Zhou
 * @Github: https://github.com/muyi2414
 * @Date: 2020-03-05 11:13:00
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2020-03-06 16:32:12
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "linuxTool.h"
#include "bmp.h"

#define ROW 8
#define COL 8

int main(int argc, char const *argv[])
{
	unsigned char origin[ROW][COL] =
	{
	    1,   5, 255, 255, 100, 200, 255, 200,
	    1,   7, 254, 255, 100,  10,  10,   9,
	    3,   7,  10, 100, 100,   2,   9,   6, 
	    3,   6,  10,  10,   9,   2,   8,   2,
	    2,   1,   8,   8,   9,   3,   4,   2,
	    1,   0,   7,   8,   8,   3,   2,   1,
	    1,   1,   8,   8,   7,   2,   2,   1,
	    2,   3,   9,   8,   7,   2,   2,   0
	};
	FullPath path_origin(argv[0], "origin.bmp");
	BMP bmp_origin(GARY, (unsigned char *)origin, COL, ROW);
	bmp_origin.write(path_origin.getFullPath());

	// 1. 线性对比度展宽，要求展宽后的对比度大于原图像的对比度
	printf("\n1. 线性对比度展宽，要求展宽后的对比度大于原图像的对比度\n");
	unsigned char image1[ROW][COL];
	for (int r = 0; r < ROW; ++r)
	{
		for (int c = 0; c < COL; ++c)
		{
			if (origin[r][c] <= 10) {
				image1[r][c] = origin[r][c] * 2;
			}
			else {
				image1[r][c] = origin[r][c] / 2 + 128;
			}
			printf("%3d, ", image1[r][c]);
		}
		printf("\n");
	}
	FullPath path_image1(argv[0], "image1.bmp");
	BMP bmp_image1(GARY, (unsigned char *)image1, COL, ROW);
	bmp_image1.write(path_image1.getFullPath());

	// 2. 对其进行非线性动态范围调整，并计算调整后图像的对比度
	printf("\n2. 对其进行非线性动态范围调整，并计算调整后图像的对比度\n");
	unsigned char image2[ROW][COL];
	for (int r = 0; r < ROW; ++r)
	{
		for (int c = 0; c < COL; ++c)
		{
			image2[r][c] = round(106 * log10(1 + origin[r][c]));
			printf("%3d, ", image2[r][c]);
		}
		printf("\n");
	}
	FullPath path_image2(argv[0], "image2.bmp");
	BMP bmp_image2(GARY, (unsigned char *)image2, COL, ROW);
	bmp_image2.write(path_image2.getFullPath());

	// 3. 对其进行直方图均衡化处理，并计算处理后图像的对比度
	printf("\n3. 对其进行直方图均衡化处理，并计算处理后图像的对比度\n");
	uint16_t h[256] = {0};  // 储存0～255每个像素值出现的次数
	double hs[256] = {0.0}; // 储存0～255每个像素值出现的概率
	unsigned char image3[ROW][COL];
	// 计算0～255每个像素值出现的次数
	printf("计算0～255每个像素值出现的次数\n");
	for (int r = 0; r < ROW; ++r)
	{
		for (int c = 0; c < COL; ++c)
		{
			h[origin[r][c]]++;
		}
	}
	for (int i = 0; i < 256; ++i)
	{
		printf("%d, ", h[i]);
	}
	printf("\b\n\n");
	// 计算0～255每个像素值出现的概率
	printf("计算0～255每个像素值出现的概率\n");
	for (int i = 0; i < 256; ++i) {
		hs[i] = (double)h[i] / (ROW * COL);
	}
	for (int i = 0; i < 256; ++i)
	{
		printf("%.2f, ", hs[i]);
	}
	printf("\b\n\n");
	// 计算0～255每个像素值出现的累计概率
	printf("计算0～255每个像素值出现的累计概率\n");
	for (int i = 1; i < 256; ++i) {
		hs[i] = hs[i] + hs[i-1];
	}
	for (int i = 0; i < 256; ++i)
	{
		printf("%.2f, ", hs[i]);
	}
	printf("\b\n\n");
	// 得到直方图均衡化后的新图像
	printf("得到直方图均衡化后的新图像\n");
	for (int r = 0; r < ROW; ++r)
	{
		for (int c = 0; c < COL; ++c)
		{
			if (origin[r][c] == 0) {
				image3[r][c] = 0;
			}
			else {
				image3[r][c] = round(hs[origin[r][c]] * 255);
			}
			printf("%3d, ", image3[r][c]);
		}
		printf("\n");
	}
	FullPath path_image3(argv[0], "image3.bmp");
	BMP bmp_image3(GARY, (unsigned char *)image3, COL, ROW);
	return 0;
}