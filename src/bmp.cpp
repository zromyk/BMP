/*
 * @Description:
 * @FilePath: /src/bmp.cpp
 * @Author: Wei Zhou
 * @Github: https://github.com/muyi2414
 * @Date: 2020-03-27 20:54:39
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2020-05-05 10:11:18
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "bmp.h"

inline uint32_t BMP::RGB_RED(uint32_t r, uint32_t c)
{
    return (rgb + (r) * head.info.biWidth + (c))->red;
}

inline uint32_t BMP::RGB_GREEN(uint32_t r, uint32_t c)
{
    return (rgb + (r) * head.info.biWidth + (c))->green;
}

inline uint32_t BMP::RGB_BLUE(uint32_t r, uint32_t c)
{
    return (rgb + (r) * head.info.biWidth + (c))->blue;
}

inline uint32_t BMP::RGB_GARY(uint32_t r, uint32_t c)
{
    return ((rgb + (r) * head.info.biWidth + (c))->red  + \
           (rgb + (r) * head.info.biWidth + (c))->green + \
           (rgb + (r) * head.info.biWidth + (c))->blue) / 3;
}

inline uint32_t BMP::RGB_SUM_DEL(uint32_t r1, uint32_t c1, uint32_t r2, uint32_t c2)
{
    return  (RGB_RED(r1, c1)   - RGB_RED(r2, c2))   * (RGB_RED(r1, c1)   - RGB_RED(r2, c2))   + \
            (RGB_GREEN(r1, c1) - RGB_GREEN(r2, c2)) * (RGB_GREEN(r1, c1) - RGB_GREEN(r2, c2)) + \
            (RGB_BLUE(r1, c1)  - RGB_BLUE(r2, c2))  * (RGB_BLUE(r1, c1)  - RGB_BLUE(r2, c2));
}

/**
 * @description: 构造函数
 */

BMP::BMP()
{
    memset(this, 0, sizeof(BMP));
}

/**
 * @description: 构造函数
 * @param {type}    width {uint32_t}: 图像的宽度
 *                  height {uint32_t}: 图像的高度
 */
BMP::BMP(uint32_t width, uint32_t height)
{
    setBMPInfo(RGB, width, height);
    memset(rgb, 0, sizeof(RGBInfoNode) * head.info.biWidth * head.info.biHeight);
}

/**
 * @description: 构造函数
 * @param {type}    rgbInfo {RGBInfoNode *}: 储存rgb信息的数组首地址
 *                  width {uint32_t}: 图像的宽度
 *                  height {uint32_t}: 图像的高度
 */
void BMP::create(RGBInfoNode *rgbInfo, uint32_t width, uint32_t height)
{
    setBMPInfo(RGB, width, height);
    memcpy(rgb, rgbInfo, sizeof(RGBInfoNode) * width * height);
}

BMP::BMP(RGBInfoNode *rgbInfo, uint32_t width, uint32_t height)
{
    create(rgbInfo, width, height);
}

/**
 * @description: 构造函数
 * @param {type}    rgbInfo {RGBInfoNode *}: 储存rgb信息的数组首地址
 *                  width {uint32_t}: 图像的宽度
 *                  height {uint32_t}: 图像的高度
 */
void BMP::create(ImageType type, uint8_t *colorInfo, uint32_t width, uint32_t height, uint8_t threshold)
{
    rgb  = NULL;
    gary = NULL;
    if (type == GARY)
    {
        setBMPInfo(type, width, height);
        for (uint32_t r = 0; r < head.info.biHeight; ++r)
        {
            for (uint32_t c = 0; c < head.info.biWidth; ++c)
            {
                *(gary + r * head.info.biWidth + c) = *(colorInfo + r * head.info.biWidth + c);
            }
        }
    }
    else if (type == BINARY)
    {
        setBMPInfo(type, width, height);
        for (uint32_t r = 0; r < head.info.biHeight; ++r)
        {
            for (uint32_t c = 0; c < head.info.biWidth; ++c)
            {
                if (*(colorInfo + r * head.info.biWidth + c) < threshold) {
                    *(gary + r * head.info.biWidth + c) = 0;
                }
                else {
                    *(gary + r * head.info.biWidth + c) = 1;
                }
            }
        }
    }
    else {
        std::cout << __FILE__ << "," << __func__ << "," << __LINE__ << " Please set image type: GARY or BINARY.\n" << std::endl;
        exit(-1);
    }
}

BMP::BMP(ImageType type, uint8_t *colorInfo, uint32_t width, uint32_t height, uint8_t threshold)
{
    create(type, colorInfo, width, height, threshold);
}

/**
 * @description:
 * @param {type}    rgbInfo {RGBInfoNode *}: 储存rgb信息的数组首地址
 *                  width {uint32_t}: 图像的宽度
 *                  height {uint32_t}: 图像的高度
 * @return:
 */
void BMP::setBMPInfo(ImageType type, uint32_t width, uint32_t height)
{
#ifdef _WIN32
    bfType                    = 19778;      // 2byte，位图文件的类型，标识，就是“BM”二字
#elif TARGET_OS_MAC
    head.file.bfType          = 19778;      // 2byte，位图文件的类型，标识，就是“BM”二字
#endif
    head.file.bfReserved      = 0x00;       // 4byte，位图文件保留字，必须为0
    head.file.bfOffBits       = 138;        // 4byte，偏移数，即位图文件头+位图信息头+调色板的大小
    head.info.biSize          = 124;        // 4byte，位图信息头的大小
    head.info.biWidth         = width;      // 4byte，位图的宽度，以像素为单位
    head.info.biHeight        = height;     // 4byte，位图的高度，以像素为单位
    head.info.biPlanes        = 0x01;       // 2byte，目标设备的级别，固定值1
    head.info.biBitCount      = 24;         // 2byte，每个像素所需的位数，1-黑白图，4-16色，8-256色，24-真彩色
    head.info.biCompression   = 0x00;       // 4byte，压缩类型，0(不压缩)，1(BI_RLE8压缩类型)，2(BI_RLE4压缩类型)
    head.info.biXPelsPerMeter = 2834;       // 4byte，水平分辨率(像素/米)
    head.info.biYPelsPerMeter = 2834;       // 4byte，垂直分辨率(像素/米)
    head.info.biClrUsed       = 0x00;       // 4byte，位图使用的颜色数，如果为0，则颜色数为2的biBitCount次方
    head.info.biClrImportant  = 0x00;       // 4byte，重要的颜色数，0代表所有颜色都重要

    this->type = type;
    applyArrayRGB();    // 指向RGB数据的二位数组头指针

    this->lineLegnth    = (head.info.biWidth*head.info.biBitCount+31)/32*4; // 扫描的单行数据长度
    this->width         = head.info.biWidth;    // BMP位图的长度
    this->height        = head.info.biHeight;   // BMP位图的高度
    this->offset        = head.file.bfOffBits;  // 偏移数，即位图文件头+位图信息头+调色板的大小
    this->redundant     = head.info.biWidth % 4;// 补齐4字节需要的字节数

    head.info.biSizeImage = head.info.biWidth * head.info.biHeight * 3
                            + head.info.biHeight * redundant;           // 4byte，全部像素占用的字节数，BI_RGB时可设为0
    head.file.bfSize = 138 + head.info.biSizeImage;                     // 4byte，位图文件的大小

    sizeImage   = head.file.bfSize;     // 全部像素占用的字节数
}

BMP::~BMP()
{
    deleteArrayRGB();
}

/**
 * @description: 申请RGB动态数组内存
 * @param {type}
 */
void BMP::applyArrayRGB()
{
    deleteArrayRGB();
    if (type == RGB) {
        rgb = new RGBInfoNode[head.info.biHeight * head.info.biWidth];
        if (rgb == NULL) {
            printf("Fatal: Malloc failed to allocate memory. Memory release failure.\n");
            exit(-1);
        }
    }
    else if (type == GARY || type == BINARY) {
        gary = new uint8_t[head.info.biHeight * head.info.biWidth];
        if (gary == NULL) {
            printf("Fatal: Malloc failed to allocate memory. Memory release failure.\n");
            exit(-1);
        }
    }
}

/**
 * @description: 撤销RGB动态数组内存
 * @param {type}
 */
void BMP::deleteArrayRGB()
{
    if (rgb) {
        delete[] rgb;
        rgb = NULL;
    }
    if (gary) {
        delete[] gary;
        gary = NULL;
    }
}

/**
 * @description: 得到BMP图像的RGB信息
 * @param {type} rgbInfo {RGBInfoNode *}: 储存RGB信息的首地址
 * @return:
 */
bool BMP::getPointInfo(RGBInfoNode* rgbInfo, uint32_t width, uint32_t height)
{
    if (type == RGB) {
        if (rgb == NULL) {
            std::cout << __FILE__ << "," << __func__ << "," << __LINE__ << " The BMP image lacks RGB information." << std::endl;
            return false;
        }
        if (width < head.info.biWidth && height < head.info.biHeight) {
            *rgbInfo = *(rgb + height * head.info.biWidth + width);
        }
        return true;
    }
    else if (type == GARY || type == BINARY)
    {
        if (gary == NULL) {
            std::cout << __FILE__ << "," << __func__ << "," << __LINE__ << " The BMP image lacks gary information." << std::endl;
            return false;
        }
        uint8_t colorInfo = *(gary + height * head.info.biWidth + width);

        rgbInfo->red   = colorInfo;
        rgbInfo->green = colorInfo;
        rgbInfo->blue  = colorInfo;
        return true;
    }
    return false;
}

bool BMP::getPointInfo(uint8_t* colorInfo, uint32_t width, uint32_t height)
{
    if (type == GARY || type == BINARY)
    {
        if (gary == NULL) {
            std::cout << __FILE__ << "," << __func__ << "," << __LINE__ << " The BMP image lacks gary information." << std::endl;
            return false;
        }
        *colorInfo = *(gary + height * head.info.biWidth + width);
        return true;
    }
    return false;
}

/**
 * @description: 得到BMP图像的颜色信息
 * @param {type} colorInfo {uint8_t *}: 储存颜色信息的首地址
 * @return: {bool} true: succeed, false: fail
 */
bool BMP::getColorInfo(RGBInfoNode* rgbInfo, bool show)
{
    if (type == RGB)
    {
        if (rgb == NULL) {
            std::cout << __FILE__ << "," << __func__ << "," << __LINE__ << " The BMP image lacks RGB information." << std::endl;
            return false;
        }
        memcpy(rgbInfo, rgb, sizeof(RGBInfoNode) * head.info.biHeight * head.info.biWidth);
        if (show == true) { showColorInfo(); }
        return true;
    }
    return false;
}

bool BMP::getColorInfo(uint8_t *colorInfo, bool show)
{
    if (type == GARY || type == BINARY)
    {
        if (gary == NULL) {
            std::cout << __FILE__ << "," << __func__ << "," << __LINE__ << " The BMP image lacks gary information." << std::endl;
            return false;
        }
        memcpy(colorInfo, gary, sizeof(uint8_t) * head.info.biHeight * head.info.biWidth);
        if (show == true) { showColorInfo(); }
        return true;
    }
    return false;
}

void BMP::showColorInfo()
{
    if (type == RGB)
    {
        if (rgb)
        {
            RGBInfoNode *temp;
            for (uint32_t r = 0; r < head.info.biHeight; ++r)
            {
                for (uint32_t c = 0; c < head.info.biWidth; ++c)
                {
                    temp = rgb + r * head.info.biWidth + c;
                    printf("%3d,%3d,%3d,", temp->red, temp->green, temp->blue);
                }
                printf("\n");
            }
        }
    }
    else if (type == GARY || type == BINARY)
    {
        if (gary)
        {
            for (uint32_t r = 0; r < head.info.biHeight; ++r)
            {
                for (uint32_t c = 0; c < head.info.biWidth; ++c)
                {
                    printf("%3d,", *(gary + r * head.info.biWidth + c));
                }
                printf("\n");
            }
        }
    }
}

void BMP::setPoint(RGBInfoNode &rgbInfo, uint32_t width, uint32_t height)
{
    if (type == RGB)
    {
        if (rgb) {
            if (width < head.info.biWidth && height < head.info.biHeight)
            {
                *(rgb + height * head.info.biWidth + width) = rgbInfo;
            }
        }
    }
}

void BMP::setPoint(uint8_t colorInfo, uint32_t width, uint32_t height)
{
    if (type == GARY || type == BINARY)
    {
        if (gary) {
            if (width < head.info.biWidth && height < head.info.biHeight)
            {
                *(gary + height * head.info.biWidth + width) = colorInfo;
            }
        }
    }
}

bool BMP::converImageType(ImageType type)
{
    if (this->type == RGB)
    {
        if (type == GARY)
        {
            if (rgb)
            {
                this->type = type;

                // 先确保释放 gary 指向 NULL
                if (gary) {
                    delete[] gary;
                    gary = NULL;
                }

                // 申请保存灰度的动态内存
                gary = new uint8_t[head.info.biHeight * head.info.biWidth];
                if (gary == NULL) {
                    printf("Fatal: Malloc failed to allocate memory. Memory release failure.\n");
                    return false;
                }

                // RGB 转换成 GARY
                RGBInfoNode* temp;
                for (uint32_t r = 0; r < head.info.biHeight; ++r)
                {
                    for (uint32_t c = 0; c < head.info.biWidth; ++c)
                    {
                        temp = rgb + r * head.info.biWidth + c;
                        *(gary + r * head.info.biWidth + c) = (temp->red + temp->green + temp->blue) / 3;
                    }
                }

                // 释放 rgb 指向的内存地址
                if (rgb) {
                    delete[] rgb;
                    rgb = NULL;
                }
                return true;
            }
        }
    }
    else if (this->type == BINARY)
    {
        if (type == GARY)
        {
            if (gary)
            {
                this->type = type;

                // BINARY 转换成 GARY
                for (uint32_t r = 0; r < head.info.biHeight; ++r)
                {
                    for (uint32_t c = 0; c < head.info.biWidth; ++c)
                    {
                        if (*(gary + r * head.info.biWidth + c))
                        {
                            *(gary + r * head.info.biWidth + c) = 255;
                        }
                        else
                        {
                            *(gary + r * head.info.biWidth + c) = 0;
                        }
                    }
                }
                return true;
            }
        }
    }
    return false;
}

/**
 * @description: 清空图像rgb信息
 * @return: {bool} true: succeed, false: fail
 */
bool BMP::clear()
{
    if (rgb && head.info.biWidth > 0 && head.info.biHeight > 0)
    {
        memset(rgb, 0, sizeof(RGBInfoNode) * head.info.biWidth * head.info.biHeight);
        return true;
    }
    return false;
}

/**
 * @description: 得到图像的对比度
 * @param {type}
 * @return: {double} 图像的对比度
 */
double BMP::getContrast()
{
    calculateContrast();
    return contrast;
}

/**
 * @description: 显示bmp位图的头结构信息
 * @param {type}
 * @return:
 */
void BMP::bitmap_file_show()
{
    printf("--------------------------------------\n");
    printf("bitmap-file header\n\n");
#ifdef _WIN32
    printf("bfType: %7c%c (%d)\n", bfType/256, bfType%256, bfType);
#elif TARGET_OS_MAC
    printf("bfType: %7c%c (%d)\n", head.file.bfType/256, head.file.bfType%256, head.file.bfType);
#endif
    if (head.file.bfSize/1024/1024 > 1024) {
        printf("bfSize: %8.1f GB (%d B)\n", head.file.bfSize / 1024.0 / 1024.0 / 1024.0, head.file.bfSize);
    }
    else if (head.file.bfSize/1024 > 1024) {
        printf("bfSize: %8.1f MB (%d B)\n", head.file.bfSize / 1024.0 / 1024.0, head.file.bfSize);
    }
    else if (head.file.bfSize > 1024) {
        printf("bfSize: %8.1f kB (%d B)\n", head.file.bfSize / 1024.0, head.file.bfSize);
    }
    else {
        printf("bfSize: %8d B\n", head.file.bfSize);
    }
    printf("bfReserved: %4d\n", head.file.bfReserved);
    printf("bfOffBits: %5d\n", head.file.bfOffBits);
}

/**
 * @description: 显示bmp位图的图像数据头结构信息
 * @param {type}
 * @return:
 */
void BMP::bitmap_info_show()
{
    printf("--------------------------------------\n");
    printf("bitmap-information header\n\n");
    printf("biSize: %15d B\n", head.info.biSize);
    printf("biWidth: %14d pixel\n", head.info.biWidth);
    printf("biHeight: %13d pixel\n", head.info.biHeight);
    printf("biPlanes: %13d\n", head.info.biPlanes);
    printf("biBitCount: %11d b deep\n", head.info.biBitCount);
    switch(head.info.biCompression)
    {
        case 0: printf("biCompression: %8d (no compression)\n", head.info.biCompression); break;
        case 1: printf("biCompression: %8d (BI_RLE8 compression type)\n", head.info.biCompression); break;
        case 2: printf("biCompression: %8d (BI_RLE4 compression type)\n", head.info.biCompression); break;
        default: printf("biCompression: %8d (error)\n", head.info.biCompression); break;
    }
    if (head.info.biSizeImage/1024/1024 > 1024) {
        printf("biSizeImage: %10.1f GB (%d B)\n", head.info.biSizeImage / 1024.0 / 1024.0 / 1024.0, head.info.biSizeImage);
    }
    else if (head.info.biSizeImage/1024 > 1024) {
        printf("biSizeImage: %10.1f MB (%d B)\n", head.info.biSizeImage / 1024.0 / 1024.0, head.info.biSizeImage);
    }
    else if (head.info.biSizeImage > 1024) {
        printf("biSizeImage: %10.1f kB (%d B)\n", head.info.biSizeImage / 1024.0, head.info.biSizeImage);
    }
    else {
        printf("biSizeImage: %10d B\n", head.info.biSizeImage);
    }
    printf("biXPelsPerMeter: %6u Pixel/m\n", head.info.biXPelsPerMeter);
    printf("biYPelsPerMeter: %6u Pixel/m\n", head.info.biYPelsPerMeter);
    printf("biClrUsed: %12u\n", head.info.biClrUsed);
    printf("biClrImportant:  %6u\n", head.info.biClrImportant);
}

/**
 * @description: 显示bmp位图的图像信息
 * @param {type}
 * @return:
 */
void BMP::imageInfo_show()
{
    printf("--------------------------------------\n");
    printf("contrast: %13.2f\n", contrast);
    printf("--------------------------------------\n");
}

/**
 * @description: 计算图像的对比度
 * @param {type}
 * @return:
 */
bool BMP::calculateContrast()
{
    if (rgb == NULL) {
        printf("Fatal: Missing RGB information.\n");
        return false;
    }
    if (height == 0) {
        std::cout << __FILE__ << "," << __func__ << "," << __LINE__ << " The height of the image is 0 pixels." << std::endl;
        return false;
    }
    if (width == 0) {
        std::cout << __FILE__ << "," << __func__ << "," << __LINE__ << " The width of the image is 0 pixels." << std::endl;
        return false;
    }

    uint32_t contrastTemp = 0;
    uint32_t pair = 0;

    // 四角对比度
    if (height > 1) {
        contrastTemp += RGB_SUM_DEL(0, 0, 1, 0);
        contrastTemp += RGB_SUM_DEL(height-1, 0, height-2, 0);
        pair += 2;
        if (width > 1) {
            contrastTemp += RGB_SUM_DEL(0, width-1, 1, width-1);
            contrastTemp += RGB_SUM_DEL(height-1, width-1, height-2, width-1);
            pair += 2;
        }
    }
    if (width > 1) {
        contrastTemp += RGB_SUM_DEL(0, 0, 0, 1);
        contrastTemp += RGB_SUM_DEL(0, width-1, 0, width-2);
        pair += 2;
        if (height > 1) {
            contrastTemp += RGB_SUM_DEL(height-1, 0, height-1, 1);
            contrastTemp += RGB_SUM_DEL(height-1, width-1, height-1, width-2);
            pair += 2;
        }
    }
    if (height > 2 && width > 2) {
        pair += (height-2) * 6;
        pair += (width-2) * 6;
        pair += (width-2) * (width-2) * 4;
    }
    contrast = static_cast<double>(contrastTemp) / 3 / pair;

    // 边缘对比度
    for (uint32_t r = 1; r < height-1; ++r)
    {
        contrastTemp  = RGB_SUM_DEL(r, 0, r-1, 0);
        contrastTemp += RGB_SUM_DEL(r, 0, r+1, 0);
        contrastTemp += RGB_SUM_DEL(r, 0, r, 1);
        contrastTemp += RGB_SUM_DEL(r, width-1, r-1, width-1);
        contrastTemp += RGB_SUM_DEL(r, width-1, r+1, width-1);
        contrastTemp += RGB_SUM_DEL(r, width-1, r, width-2);
        contrast += static_cast<double>(contrastTemp) / 3 / pair;

    }
    for (uint32_t c = 1; c < width-1; ++c)
    {
        contrastTemp  = RGB_SUM_DEL(0, c, 0, c-1);
        contrastTemp += RGB_SUM_DEL(0, c, 0, c+1);
        contrastTemp += RGB_SUM_DEL(0, c, 1, c);
        contrastTemp += RGB_SUM_DEL(height-1, c, height-1, c-1);
        contrastTemp += RGB_SUM_DEL(height-1, c, height-1, c+1);
        contrastTemp += RGB_SUM_DEL(height-1, c, height-2, c);
        contrast += static_cast<double>(contrastTemp) / 3 / pair;

    }

    // 中间区域对比度
    for (uint32_t r = 1; r < height-1; ++r)
    {
        for (uint32_t c = 1; c < width-1; ++c)
        {
            contrastTemp  = RGB_SUM_DEL(r, c, r-1, c);
            contrastTemp += RGB_SUM_DEL(r, c, r+1, c);
            contrastTemp += RGB_SUM_DEL(r, c, r, c-1);
            contrastTemp += RGB_SUM_DEL(r, c, r, c+1);
            contrast += static_cast<double>(contrastTemp) / 3 / pair;
        }
    }
    return true;
}

bool BMP::linear_contrast_expansion(uint8_t a, uint8_t b, uint8_t min, uint8_t max)
{
    if (type == RGB)
    {
        // ...
        return true;
    }
    else if (type == GARY)
    {
        uint8_t *temp;
        double  slope = (max - min) / (b - a);

        for (uint32_t r = 0; r < head.info.biHeight; ++r)
        {
            for (uint32_t c = 0; c < head.info.biWidth; ++c)
            {
                temp = gary + r * head.info.biWidth + c;

                if (*temp > b)
                {
                    *temp = max;
                }
                else if (a < *temp && *temp < b)
                {
                    *temp = static_cast<uint8_t>(slope * (*temp - a) + min);
                }
                else
                {
                    *temp = min;
                }
            }
        }
        return true;
    }
    return false;
}

/**
 * @description: 读取一张mbp图像的信息
 * @param {type} filePath {const char *}: 需要读取的MBP文件的路径
 * @return: {bool} true: succeed, false: fail
 */
bool BMP::read(const char *filePath, bool showInfo)
{
    FILE *fp;
    if ((fp = fopen(filePath, "rb")) == NULL) {
        std::cout << __FILE__ << "," << __func__ << "," << __LINE__ << " Can't open " << filePath << std::endl;
        return false;
    }
#ifdef _WIN32
    fread(&bfType, sizeof(uint16_t), 1, fp);
#endif
    fread(&head, sizeof(BmpHeadNode), 1, fp);

    if (showInfo == true) {
        bitmap_file_show(); // 显示文件头结构体信息
        bitmap_info_show(); // 显示图像数据头结构体信息
    }
    lineLegnth  = (head.info.biWidth*head.info.biBitCount+31)/32*4; // 扫描的单行数据长度
    width       = head.info.biWidth;    // BMP位图的长度
    height      = head.info.biHeight;   // BMP位图的高度
    sizeImage   = head.file.bfSize;     // 全部像素占用的字节数
    offset      = head.file.bfOffBits;  // 偏移数，即位图文件头+位图信息头+调色板的大小
    redundant   = head.info.biWidth % 4; // 补齐4字节需要的字节数

    // 限制读取的图像长宽
    if (head.info.biHeight > 32768 || head.info.biWidth > 32768) {
        std::cout << __FILE__ << "," << __func__ << "," << __LINE__ << " The image is too large. Please check if the image is damaged." << std::endl;
    }
    else {
        applyArrayRGB();    // 指向RGB数据的二位数组头指针
        for (uint32_t iy = 0; iy < head.info.biHeight; ++iy) {
            //因为bmp文件的原点是左下角，所以bmp图片需要顺着y轴的反方向来读取
            fseek(fp, head.file.bfOffBits + (head.info.biHeight-1 - iy) * lineLegnth, SEEK_SET); //最后一行开始读取
            fread(rgb + iy * head.info.biWidth, sizeof(RGBInfoNode), head.info.biWidth, fp);
        }
        if (showInfo == true) {
            imageInfo_show();
        }
    }
    fclose(fp);
    return true;
}

/**
 * @description: 生成一张BMP图片
 * @param {type} filePath {const char *}: 需要读取的MBP文件的路径
 * @return: {bool} true: succeed, false: fail
 */
bool BMP::write(const char *filePath, bool showInfo)
{
    FILE *fp;

    if ((fp = fopen(filePath, "wb")) == NULL) {
        std::cout << __FILE__ << "," << __func__ << "," << __LINE__ << " Can't build " << filePath << std::endl;
        return false;
    }
    if (rgb == NULL && gary == NULL) { // 如果没有rgb图像信息，发送错误消息
        std::cout << __FILE__ << "," << __func__ << "," << __LINE__ << " Unable to find image information." << std::endl;
        return false;
    }

    if (showInfo == true) {
        bitmap_file_show(); // 显示文件头结构体信息
        bitmap_info_show(); // 显示图像数据头结构体信息
        imageInfo_show();
    }

    uint32_t ix = 0;
#ifdef _WIN32
    fwrite(&bfType, sizeof(uint16_t), 1, fp);
    ix += sizeof(uint16_t);
#endif
    fwrite(&head, sizeof(BmpHeadNode), 1, fp);
    ix += sizeof(BmpHeadNode);
    for (; ix < head.file.bfOffBits; ++ix) {
        fputc(0x00, fp);
    }

    if (type == RGB)
    {
        for (uint32_t iy = 0; iy < head.info.biHeight; ++iy) {
            fwrite((rgb + (head.info.biHeight-1 - iy) * head.info.biWidth), sizeof(RGBInfoNode), head.info.biWidth, fp);
            for (uint32_t ix = 0; ix < redundant; ++ix) {
                fputc(0x00, fp);
            }
        }
    }
    else if (type == GARY || type == BINARY)
    {
        RGBInfoNode *rgb = new RGBInfoNode[head.info.biHeight * head.info.biWidth];
        if (rgb == NULL) {
            printf("Fatal: Malloc failed to allocate memory. Memory release failure.\n");
            return false;
        }

        RGBInfoNode* temp_rgb;
        uint8_t temp_gary;

        if (type == GARY)
        {
            for (uint32_t r = 0; r < head.info.biHeight; ++r)
            {
                for (uint32_t c = 0; c < head.info.biWidth; ++c)
                {
                    temp_rgb = rgb + r * head.info.biWidth + c;
                    temp_gary = *(gary + r * head.info.biWidth + c);

                    temp_rgb->red   = temp_gary;
                    temp_rgb->green = temp_gary;
                    temp_rgb->blue  = temp_gary;
                }
            }
        }
        else
        {
            for (uint32_t r = 0; r < head.info.biHeight; ++r)
            {
                for (uint32_t c = 0; c < head.info.biWidth; ++c)
                {
                    temp_rgb = rgb + r * head.info.biWidth + c;
                    temp_gary = *(gary + r * head.info.biWidth + c);

                    if (temp_gary == 0) {
                        temp_rgb->red   = 0;
                        temp_rgb->green = 0;
                        temp_rgb->blue  = 0;
                    }
                    else {
                        temp_rgb->red   = 255;
                        temp_rgb->green = 255;
                        temp_rgb->blue  = 255;
                    }
                }
            }
        }

        for (uint32_t iy = 0; iy < head.info.biHeight; ++iy) {
            fwrite((rgb + (head.info.biHeight-1 - iy) * head.info.biWidth), sizeof(RGBInfoNode), head.info.biWidth, fp);
            for (uint32_t ix = 0; ix < redundant; ++ix) {
                fputc(0x00, fp);
            }
        }

        delete[] rgb;
    }

    fclose(fp);
    return true;
}
