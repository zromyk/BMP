/*
 * @Description: 
 * @FilePath: /BMP/src/main.cpp
 * @Author: Wei Zhou
 * @Github: https://github.com/muyi2414
 * @Date: 2020-03-05 11:13:00
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2021-01-23 14:14:52
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */

#include <iostream>
#include "bmp.h"

#define test(str, func) \
    do { \
        printf("test %-24s", str); \
        if (func) {printf("Succeed.\n");} \
        else     {printf("Failure!!!\n");} \
    } while(0)

void test_func(void)
{
    unsigned char origin[][8] =
    {
        {1,   5, 255, 255, 100, 200, 255, 200},
        {1,   7, 254, 255, 100,  10,  10,   9},
        {3,   7,  10, 100, 100,   2,   9,   6}, 
        {3,   6,  10,  10,   9,   2,   8,   2},
        {2,   1,   8,   8,   9,   3,   4,   2},
        {1,   0,   7,   8,   8,   3,   2,   1},
        {1,   1,   8,   8,   7,   2,   2,   1},
        {2,   3,   9,   8,   7,   2,   2,   0}
    };

    /* 创建一个 BMP 图片对象，并将 origin 填入到图片中 */
    BMP bmp_write(
        (unsigned char *)origin, 
        sizeof(origin)/sizeof(origin[0])/sizeof(unsigned char), 
        sizeof(origin[0])/sizeof(unsigned char));
    bmp_write.write("/Users/zhouwei/Desktop/BMP/bmp_write.bmp", true);

    /* 创建一个 BMP 图片对象，读取图片并保存在内 */
    BMP bmp_read;
    bmp_read.read("/Users/zhouwei/Desktop/BMP/bmp_write.bmp", true);

    printf("------------------------------------------------------\n");
    uint8_t garyInfo;
    uint8_t garyInfo_screenShot[2][2];
    uint8_t garyInfo_fill[2][2] = {255, 255, 255, 255};

    // 注意读取的 BMP 图片为 RGB 格式，请转化成 GARY 格式
    test("rgb2gary()",          bmp_read.rgb2gary());
    test("getPoint()",          bmp_read.getPoint(&garyInfo, 2, 2));
    test("setPoint()",          bmp_read.setPoint(255, 3, 3));  
        bmp_read.write("/Users/zhouwei/Desktop/BMP/bmp_write_setPoint.bmp");
    test("screenShot()",        bmp_read.screenShot(&garyInfo_screenShot[0][0], 2, 2, 2, 2));
    test("garyInfo_fill()",     bmp_read.fill(&garyInfo_fill[0][0], 2, 2, 2, 2));   
        bmp_read.write("/Users/zhouwei/Desktop/BMP/bmp_write_fill.bmp");
    test("clear()",             bmp_read.clear());   
        bmp_read.write("/Users/zhouwei/Desktop/BMP/bmp_write_clear.bmp");
}

int main(int argc, char const *argv[])
{
    test_func();
    return 0;
}