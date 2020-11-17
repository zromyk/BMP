/*
 * @Description: 
 * @FilePath: /src/Users/zhouwei/Desktop/project/cpp/BMP/include/bmp2.h
 * @Author: Wei Zhou
 * @Github: https://github.com/zromyk
 * @Date: 2020-05-05 10:12:10
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2020-05-05 10:12:10
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */
/*
 * @Description:
 * @FilePath: /SmcroIDE/bmp.h
 * @Author: Wei Zhou
 * @Github: https://github.com/muyi2414
 * @Date: 2020-03-27 09:09:45
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2020-03-29 20:56:36
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */

#pragma once

#include <iostream>
#include <cstdlib>
#include <cstring>

typedef struct
{
    uint8_t blue;     // 蓝色强度
    uint8_t green;    // 绿色强度
    uint8_t red;      // 红色强度
}__attribute__((__packed__))RGBInfoNode;

typedef enum
{
    GARY,   // 灰度图
    BINARY  // 二值化图
}ImageType;

#ifdef _WIN32

typedef struct  // BMP文件头结构，14byte
{
    //uint16_t bfType;      // 2byte，位图文件的类型，标识，就是“BM”二字
    uint32_t bfSize;        // 4byte，位图文件的大小
    uint32_t bfReserved;    // 4byte，位图文件保留字，必须为0
    uint32_t bfOffBits;     // 4byte，偏移数，即位图文件头+位图信息头+调色板的大小
}__attribute__((__packed__))BitmapFileNode;

#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_MAC
typedef struct  // BMP文件头结构，14byte
{
    uint16_t bfType;        // 2byte，位图文件的类型，标识，就是“BM”二字
    uint32_t bfSize;        // 4byte，位图文件的大小
    uint32_t bfReserved;    // 4byte，位图文件保留字，必须为0
    uint32_t bfOffBits;     // 4byte，偏移数，即位图文件头+位图信息头+调色板的大小
}__attribute__((__packed__))BitmapFileNode;
    #else
        #error "Unknown Apple platform"
    #endif
#else
    #error "Unknown compiler"
#endif

typedef struct // 图像信息区，40byte
{
    uint32_t biSize;            // 4byte，位图信息头的大小
    uint32_t biWidth;           // 4byte，位图的宽度，以像素为单位
    uint32_t biHeight;          // 4byte，位图的高度，以像素为单位
    uint16_t biPlanes;          // 2byte，目标设备的级别，固定值1
    uint16_t biBitCount;        // 2byte，每个像素所需的位数，1-黑白图，4-16色，8-256色，24-真彩色
    uint32_t biCompression;     // 4byte，压缩类型，0(不压缩)，1(BI_RLE8压缩类型)，2(BI_RLE4压缩类型)
    uint32_t biSizeImage;       // 4byte，全部像素占用的字节数，BI_RGB时可设为0
    uint32_t biXPelsPerMeter;   // 4byte，水平分辨率(像素/米)
    uint32_t biYPelsPerMeter;   // 4byte，垂直分辨率(像素/米)
    uint32_t biClrUsed;         // 4byte，位图使用的颜色数，如果为0，则颜色数为2的biBitCount次方
    uint32_t biClrImportant;    // 4byte，重要的颜色数，0代表所有颜色都重要
}__attribute__((__packed__))BitmapInfoNode;

/* BMP位图头结构体 */
typedef struct
{
    BitmapFileNode file;
    BitmapInfoNode info;
}__attribute__((__packed__))BmpHeadNode;

/* BMP位图数据 24，32位深适用*/
class BMP
{
public:

    /**
     * @description: 构造函数（不设置任何参数，下一步可以用read直接读取一张BMP图片）
     */
    BMP();
    BMP(uint32_t width, uint32_t height);

    /**
     * @description: 构造函数
     * @param {type} rgbInfo {RGBInfoNode *}: 储存rgb信息的数组首地址
     *               width {uint32_t}: 图像的宽度
     *               height {uint32_t}: 图像的高度
     */
    BMP(RGBInfoNode *rgbInfo, uint32_t width, uint32_t height);
    // 以RGB数据设置图像RGB信息
    void createBMP(RGBInfoNode *rgbInfo, uint32_t width, uint32_t height);
    // 以RGB数据设置图像 [ 单个点 ] RGB信息
    void setPoint(RGBInfoNode &rgbInfo, uint32_t col, uint32_t row);

    /**
     * @description: 构造函数
     * @param {type} type {ImageType}: GARY (灰度图); BINARY (二值化图)
     *               rgbInfo {uint8_t *}: 储存颜色信息的数组首地址
     *               width {uint32_t}: 图像的宽度
     *               height {uint32_t}: 图像的高度
     */
    BMP(ImageType type, uint8_t *colorInfo, uint32_t width, uint32_t height);
    // 以灰度数据设置图像RGB信息
    void createBMP_GARY(uint8_t *colorInfo, uint32_t width, uint32_t height);
    // 以灰度数据设置图像 [ 单个点 ] RGB信息
    void setPoint_GARY(uint8_t colorInfo, uint32_t col, uint32_t row);

    // 以二值化数据设置图像RGB信息
    void createBMP_BINARY(uint8_t *colorInfo, uint32_t width, uint32_t height);
    // 以二值化数据设置图像 [ 单个点 ] RGB信息
    void setPoint_BINARY(uint8_t colorInfo, uint32_t col, uint32_t row);

    /**
     * @description: 清空图像rgb信息
     * @return: {bool} true: succeed, false: fail
     */
    bool clear();

    ~BMP();

    /**
     * @description: 读取一张mbp图像的信息
     * @param {type} filePath {const char *}: 需要读取的MBP文件的路径
     * @return: {bool} true: succeed, false: fail
     */
    bool read(const char *filePath, bool showInfo = false);

    /**
     * @description: 生成一张BMP图片
     * @param {type} filePath {const char *}: 需要读取的MBP文件的路径
     * @return: {bool} true: succeed, false: fail
     */
    bool write(const char *filePath, bool showInfo = false);

    /**
     * @description: 得到BMP图像的RGB信息
     * @param {type} rgbInfo {RGBInfoNode *}: 储存RGB信息的首地址
     *               show {bool}: 是否显示结果
     * @return: {bool} true: succeed, false: fail
     */
    bool getInfoRGB(RGBInfoNode* rgbInfo, bool show = false);
    // 得到图像单个点RGB信息
    RGBInfoNode getPointRGB(uint32_t width, uint32_t height);
    // 输出图像所有RGB信息
    void showInfoRGB();

    /**
     * @description: 得到BMP图像的灰度
     * @param {type} garyInfo {uint8_t *}: 储存灰度信息的首地址
     *               show {bool}: 是否显示结果
     * @return: {bool} true: succeed, false: fail
     */
    bool getInfoGary(uint8_t *garyInfo, bool show = false);
    // 以灰度形式输出图像所有RGB信息
    void showInfoGary();

    /**
     * @description: 得到BMP图像的二值化信息
     * @param {type} binaryInfo {uint8_t *}: 储存二值化信息的首地址
     *               threshold {uint8_t}: 二值化阈值
     *               show {bool}: 是否显示结果
     * @return: {bool} true: succeed, false: fail
     */
    bool getInfoBinary(uint8_t *binaryInfo, uint8_t threshold = 128, bool show = false);
    // 以二值化形式输出图像所有RGB信息
    void showInfoBinary(uint8_t threshold = 128);

    /**
     * @description: 得到图像的高度
     * @return: {uint32_t} 图像的高度
     */
    inline uint32_t getHeight() {return head.info.biHeight;}

    /**
     * @description: 得到图像的宽度
     * @return: {uint32_t} 图像的宽度
     */
    inline uint32_t getWidth() {return head.info.biWidth;}

    /**
     * @description: 得到图像的对比度
     * @return: {double} 图像的对比度
     */
    double getContrast();

    inline bool isExists() {return !(rgb == NULL);}

    void minimumVariance();
    void kNeighborFilter(uint8_t k);
    void symmetryMeanFilter();
    void SigmaFilter();

private:
#ifdef _WIN32
    uint16_t bfType;
#endif
    BmpHeadNode head;       // 头结构体
    RGBInfoNode *rgb;       // 指向RGB数据的头指针

    uint32_t lineLegnth;    // 扫描的单行数据长度
    uint32_t width;         // BMP位图的长度
    uint32_t height;        // BMP位图的高度
    uint32_t sizeImage;     // 全部像素占用的字节数
    uint32_t offset;        // 偏移数，即位图文件头+位图信息头+调色板的大小
    uint32_t redundant;     // 补齐4字节需要的字节数

    double contrast;        // 对比度

    const static uint8_t TEMP_AREA = 5;
    const static uint8_t TEMP_AREA_HALF = TEMP_AREA/2;

    void bitmap_file_show();
    void bitmap_info_show();
    void imageInfo_show();
    void applyArrayRGB();
    void deleteArrayRGB();

    void setBMPInfo(uint32_t width, uint32_t height);
    bool calculateContrast();

    inline void swap(uint8_t *a, uint8_t *b);
    inline uint32_t RGB_RED(uint32_t r, uint32_t c);
    inline uint32_t RGB_GREEN(uint32_t r, uint32_t c);
    inline uint32_t RGB_BLUE(uint32_t r, uint32_t c);
    inline uint32_t RGB_GARY(uint32_t r, uint32_t c);
    inline uint32_t RGB_SUM_DEL(uint32_t r1, uint32_t c1, uint32_t r2, uint32_t c2);

    void countAverageVariance(double *mean, double *variance, uint32_t row, uint32_t col, uint8_t enable[TEMP_AREA][TEMP_AREA]);
    uint8_t countNeighborAverage(uint8_t k, uint32_t row, uint32_t col);
    uint8_t symmetryMean(uint32_t row, uint32_t col);
    uint8_t sigmaMean(uint32_t row, uint32_t col);
};
