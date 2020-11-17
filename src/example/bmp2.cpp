

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "bmp2.h"
#include "my_math.h"

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

BMP::BMP()
{
    rgb = NULL;
    memset(this, 0, sizeof(BMP));
}

BMP::BMP(uint32_t width, uint32_t height)
{
    rgb = NULL;
    setBMPInfo(width, height);
    memset(rgb, 0, sizeof(RGBInfoNode) * head.info.biWidth * head.info.biHeight);
}

/**
 * @description: 构造函数
 * @param {type}    rgbInfo {RGBInfoNode *}: 储存rgb信息的数组首地址
 *                  width {uint32_t}: 图像的宽度
 *                  height {uint32_t}: 图像的高度
 * @return:
 */
BMP::BMP(RGBInfoNode *rgbInfo, uint32_t width, uint32_t height)
{
    rgb = NULL;
    createBMP(rgbInfo, width, height);
}

BMP::~BMP()
{
    deleteArrayRGB();
}

/**
 * @description: 申请RGB动态数组内存
 * @param {type}
 * @return:
 */
void BMP::applyArrayRGB()
{
    deleteArrayRGB();
    rgb = new RGBInfoNode[head.info.biHeight * head.info.biWidth];
    if (rgb == NULL) {
        printf("Fatal: Malloc failed to allocate memory. Memory release failure.\n");
        exit(-1);
    }
}

/**
 * @description: 撤销RGB动态数组内存
 * @param {type}
 * @return:
 */
void BMP::deleteArrayRGB()
{
    if (rgb) {
        delete[] rgb;
        rgb = NULL;
    }
}

/**
 * @description:
 * @param {type}    rgbInfo {RGBInfoNode *}: 储存rgb信息的数组首地址
 *                  width {uint32_t}: 图像的宽度
 *                  height {uint32_t}: 图像的高度
 * @return:
 */
void BMP::createBMP(RGBInfoNode *rgbInfo, uint32_t width, uint32_t height)
{
    setBMPInfo(width, height);
    for (uint32_t r = 0; r < head.info.biHeight; ++r)
    {
        for (uint32_t c = 0; c < head.info.biWidth; ++c)
        {
            *(rgb + r * head.info.biWidth + c) = *(rgbInfo + r * width + c);
        }
    }
}

void BMP::setPoint(RGBInfoNode &rgbInfo, uint32_t c, uint32_t r)
{
    if (c < head.info.biWidth && r < head.info.biHeight)
    {
        *(rgb + r * head.info.biWidth + c) = rgbInfo;
    }
}

/**
 * @description: 构造函数
 * @param {type}    rgbInfo {RGBInfoNode *}: 储存rgb信息的数组首地址
 *                  width {uint32_t}: 图像的宽度
 *                  height {uint32_t}: 图像的高度
 * @return:
 */
BMP::BMP(ImageType type, uint8_t *colorInfo, uint32_t width, uint32_t height)
{
    if (type == GARY) {
        createBMP_GARY(colorInfo, width, height);
    }
    else if (type == BINARY){
        createBMP_BINARY(colorInfo, width, height);
    }
    else {
        std::cout << __FILE__ << "," << __func__ << "," << __LINE__ << " Please set image type: GARY or BINARY.\n" << std::endl;
        exit(-1);
    }
}

/**
 * @description:
 * @param {type}    rgbInfo {RGBInfoNode *}: 储存rgb信息的数组首地址
 *                  width {uint32_t}: 图像的宽度
 *                  height {uint32_t}: 图像的高度
 * @return:
 */
void BMP::createBMP_GARY(uint8_t *colorInfo, uint32_t width, uint32_t height)
{
    setBMPInfo(width, height);

    for (uint32_t r = 0; r < head.info.biHeight; ++r)
    {
        for (uint32_t c = 0; c < head.info.biWidth; ++c)
        {
            uint8_t color = *(colorInfo + r * head.info.biWidth + c);
            RGBInfoNode *rgbInfo = rgb + r * head.info.biWidth + c;
            rgbInfo->red   = color;
            rgbInfo->green = color;
            rgbInfo->blue  = color;
        }
    }
}

void BMP::setPoint_GARY(uint8_t colorInfo, uint32_t c, uint32_t r)
{
    if (c < head.info.biWidth && r < head.info.biHeight)
    {
        RGBInfoNode *rgbInfo = rgb + r * head.info.biWidth + c;
        rgbInfo->red   = colorInfo;
        rgbInfo->green = colorInfo;
        rgbInfo->blue  = colorInfo;
    }
}

/**
 * @description:
 * @param {type}    rgbInfo {RGBInfoNode *}: 储存rgb信息的数组首地址
 *                  width {uint32_t}: 图像的宽度
 *                  height {uint32_t}: 图像的高度
 * @return:
 */
void BMP::createBMP_BINARY(uint8_t *colorInfo, uint32_t width, uint32_t height)
{
    setBMPInfo(width, height);

    for (uint32_t r = 0; r < head.info.biHeight; ++r)
    {
        for (uint32_t c = 0; c < head.info.biWidth; ++c)
        {
            RGBInfoNode *rgbInfo = rgb + r * head.info.biWidth + c;
            if (*(colorInfo + r * head.info.biWidth + c) == 0) {
                rgbInfo->red   = 0;
                rgbInfo->green = 0;
                rgbInfo->blue  = 0;
            }
            else {
                rgbInfo->red   = 255;
                rgbInfo->green = 255;
                rgbInfo->blue  = 255;
            }
        }
    }
}

void BMP::setPoint_BINARY(uint8_t colorInfo, uint32_t c, uint32_t r)
{
    if (c < head.info.biWidth && r < head.info.biHeight)
    {
        RGBInfoNode *rgbInfo = rgb + r * head.info.biWidth + c;
        if (colorInfo == 0) {
            rgbInfo->red   = 0;
            rgbInfo->green = 0;
            rgbInfo->blue  = 0;
        }
        else {
            rgbInfo->red   = 255;
            rgbInfo->green = 255;
            rgbInfo->blue  = 255;
        }
    }
}

/**
 * @description:
 * @param {type}    rgbInfo {RGBInfoNode *}: 储存rgb信息的数组首地址
 *                  width {uint32_t}: 图像的宽度
 *                  height {uint32_t}: 图像的高度
 * @return:
 */
void BMP::setBMPInfo(uint32_t width, uint32_t height)
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

/**
 * @description: 得到BMP图像的RGB信息
 * @param {type} rgbInfo {RGBInfoNode *}: 储存RGB信息的首地址
 * @return:
 */
bool BMP::getInfoRGB(RGBInfoNode* rgbInfo, bool show)
{
    if (rgb == NULL) {
        std::cout << __FILE__ << "," << __func__ << "," << __LINE__ << " The BMP image lacks RGB information." << std::endl;
        return false;
    }
    memcpy(rgbInfo, rgb, sizeof(RGBInfoNode) * head.info.biHeight * head.info.biWidth);
    if (show == true)
    {
        RGBInfoNode *temp;
        for (uint32_t r = 0; r < head.info.biHeight; ++r)
        {
            for (uint32_t c = 0; c < head.info.biWidth; ++c)
            {
                temp = rgbInfo + r * head.info.biWidth + c;
                printf("%3d,%3d,%3d,", temp->red, temp->green, temp->blue);
            }
            printf("\n");
        }
    }
    return true;
}

RGBInfoNode BMP::getPointRGB(uint32_t width, uint32_t height)
{
    if (width < head.info.biWidth && height < head.info.biHeight) {
        return *(rgb + height * head.info.biWidth + width);
    }
    exit(-1);
}

void BMP::showInfoRGB()
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

/**
 * @description: 得到BMP图像的灰度信息
 * @param {type} garyInfo {uint8_t *}: 储存灰度信息的首地址
 * @return:
 */
bool BMP::getInfoGary(uint8_t *garyInfo, bool show)
{
    if (rgb == NULL) {
        std::cout << __FILE__ << "," << __func__ << "," << __LINE__ << " The BMP image lacks RGB information." << std::endl;
        return false;
    }
    RGBInfoNode *rgbInfo;
    for (uint32_t r = 0; r < head.info.biHeight; ++r)
    {
        for (uint32_t c = 0; c < head.info.biWidth; ++c)
        {
            rgbInfo = rgb + r * head.info.biWidth + c;
            *(garyInfo + r * head.info.biWidth + c)
                = (rgbInfo->red + rgbInfo->green + rgbInfo->blue) / 3;
        }
    }
    if (show == true)
    {
        for (uint32_t r = 0; r < head.info.biHeight; ++r)
        {
            for (uint32_t c = 0; c < head.info.biWidth; ++c)
            {
                printf("%3d,", *(garyInfo + r * head.info.biWidth + c));
            }
            printf("\n");
        }
    }
    return true;
}

void BMP::showInfoGary()
{
    RGBInfoNode *rgbInfo;
    for (uint32_t r = 0; r < head.info.biHeight; ++r)
    {
        for (uint32_t c = 0; c < head.info.biWidth; ++c)
        {
            rgbInfo = rgb + r * head.info.biWidth + c;
            printf("%3d,", (rgbInfo->red + rgbInfo->green + rgbInfo->blue) / 3);
        }
        printf("\n");
    }
}

/**
* @description: 得到BMP图像的二值化信息
* @param {type} binaryInfo {uint8_t *}: 储存二值化信息的首地址
*               threshold {uint8_t}: 二值化阈值
* @return: {bool} true: succeed, false: fail
*/
bool BMP::getInfoBinary(uint8_t *binaryInfo, uint8_t threshold, bool show)
{
    if (rgb == NULL) {
        std::cout << __FILE__ << "," << __func__ << "," << __LINE__ << " The BMP image lacks RGB information." << std::endl;
        return false;
    }
    RGBInfoNode *rgbInfo;
    for (uint32_t r = 0; r < head.info.biHeight; ++r)
    {
        for (uint32_t c = 0; c < head.info.biWidth; ++c)
        {
            rgbInfo = rgb + r * head.info.biWidth + c;
            if ((rgbInfo->red + rgbInfo->green + rgbInfo->blue) / 3 < threshold) {
                *(binaryInfo + r * head.info.biWidth + c) = 0;
            }
            else {
                *(binaryInfo + r * head.info.biWidth + c) = 1;
            }
        }
    }
    if (show == true)
    {
        for (uint32_t r = 0; r < head.info.biHeight; ++r)
        {
            for (uint32_t c = 0; c < head.info.biWidth; ++c)
            {
                printf("%d,", *(binaryInfo + r * head.info.biWidth + c));
            }
            printf("\n");
        }
    }
    return true;
}

void BMP::showInfoBinary(uint8_t threshold)
{
    RGBInfoNode *rgbInfo;
    for (uint32_t r = 0; r < head.info.biHeight; ++r)
    {
        for (uint32_t c = 0; c < head.info.biWidth; ++c)
        {
            rgbInfo = rgb + r * head.info.biWidth + c;
            if ((rgbInfo->red + rgbInfo->green + rgbInfo->blue) / 3 < threshold) {
                printf("0,");
            }
            else {
                printf("1,");
            }
        }
        printf("\n");
    }
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

void BMP::countAverageVariance(double *mean, double *variance, uint32_t row, uint32_t col, uint8_t enable[TEMP_AREA][TEMP_AREA])
{
    uint32_t len = 0;
    uint32_t sumGary = 0;
    double sumPoor2 = 0;

    for (int8_t r = 0; r < TEMP_AREA; ++r)
    {
        for (int8_t c = 0; c < TEMP_AREA; ++c)
        {
            if (*(enable + r * TEMP_AREA + c))
            {
                len++;
                sumGary += RGB_GARY(row+r-TEMP_AREA_HALF, col+c-TEMP_AREA_HALF);
            }
        }
    }
    *mean = (double)sumGary / len;

    for (int8_t r = 0; r < TEMP_AREA; ++r)
    {
        for (int8_t c = 0; c < TEMP_AREA; ++c)
        {
            if (*(enable + r * TEMP_AREA + c))
            {
                sumPoor2 += (RGB_GARY(row+r-TEMP_AREA_HALF, col+c-TEMP_AREA_HALF) - *mean) 
                            * (RGB_GARY(row+r-TEMP_AREA_HALF, col+c-TEMP_AREA_HALF) - *mean);
            }
        }
    }
    *variance = sumPoor2 / len;
}

void BMP::minimumVariance()
{
    uint8_t enable[9][TEMP_AREA][TEMP_AREA] = 
    {
        0, 0, 0, 0, 0,
        1, 1, 0, 0, 0,
        1, 1, 1, 0, 0,
        1, 1, 0, 0, 0,
        0, 0, 0, 0, 0,

        0, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 
        0, 0, 1, 1, 1, 
        0, 0, 0, 1, 1,
        0, 0, 0, 0, 0,

        0, 1, 1, 1, 0, 
        0, 1, 1, 1, 0, 
        0, 0, 1, 0, 0, 
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,

        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 1, 1, 1, 0,
        0, 1, 1, 1, 0,

        0, 0, 0, 0, 0,
        1, 1, 0, 0, 0,
        1, 1, 1, 0, 0,
        0, 1, 1, 0, 0,
        0, 0, 0, 0, 0,

        0, 0, 0, 0, 0,
        0, 0, 1, 1, 0, 
        0, 0, 1, 1, 1, 
        0, 0, 0, 1, 1,
        0, 0, 0, 0, 0,

        0, 0, 1, 1, 0,
        0, 1, 1, 1, 0,
        0, 1, 1, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,

        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 1, 1, 0, 0,
        0, 1, 1, 1, 0,
        0, 0, 1, 1, 0,

        0, 0, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 1, 1, 1, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 0, 0
    };

    double mean[9];
    double variance[9];
    double minVariance;
    uint8_t sign;
    uint8_t gary;

    for (uint32_t r = 0; r < head.info.biHeight; ++r)
    {
        if (TEMP_AREA_HALF-1 < r && r < head.info.biHeight-TEMP_AREA_HALF) 
        {
            for (uint32_t c = 0; c < head.info.biWidth; ++c)
            {
                if (TEMP_AREA_HALF-1 < c && c < head.info.biWidth-TEMP_AREA_HALF) 
                {
                    countAverageVariance(&mean[0], &variance[0], r, c, enable[0]);
                    minVariance = variance[0];
                    sign = 0;

                    for (uint8_t i = 1; i < 9; ++i) {
                        countAverageVariance(&mean[i], &variance[i], r, c, enable[i]);
                        if (minVariance > variance[i]) { 
                            minVariance = variance[i];
                            sign = i; 
                        }
                    }
                    printf("%3d,", (uint8_t)round(mean[sign]));
                }
                else {
                    printf("%3d,", RGB_GARY(r, c));
                }
            }
        }
        else {
            for (uint32_t c = 0; c < head.info.biWidth; ++c)
            {
                printf("%3d,", RGB_GARY(r, c));
            }
        }
        printf("\n");
    }
}

inline void BMP::swap(uint8_t *a, uint8_t *b)
{
    uint8_t temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

uint8_t BMP::countNeighborAverage(uint8_t k, uint32_t row, uint32_t col)
{
    const uint8_t NUM = 8;
    uint8_t gary_center, gary[NUM];

    gary_center = RGB_GARY(row, col); // 第一个点为中心
    gary[0] = RGB_GARY(row-1, col-1);
    gary[1] = RGB_GARY(row-1, col  );
    gary[2] = RGB_GARY(row-1, col+1);
    gary[3] = RGB_GARY(row,   col-1);
    gary[4] = RGB_GARY(row,   col+1);
    gary[5] = RGB_GARY(row+1, col-1);
    gary[6] = RGB_GARY(row+1, col  );
    gary[7] = RGB_GARY(row+1, col+1);

    /* 排序 */
    uint8_t cout;
    for (uint8_t i = 0; i < NUM-1; ++i)
    {
        cout = 0;
        for (uint8_t j = 0; j < NUM-1 - i; ++j)
        {
            if (_abs(gary[j] - gary_center) > _abs(gary[j+1] - gary_center)) {
                swap(&gary[j], &gary[j+1]);
                cout++;
            }
        }
        if (cout == 0)
        {
            break;
        }
    }
    // printf("sorting: %3d, ", gary_center);
    // for (uint8_t i = 0; i < NUM; ++i) {
    //     printf("%3d, ", gary[i]);
    // }

    /* 计算平均值 */
    double sum = gary_center;

    for (uint8_t i = 0; i < k-1; ++i) {
        sum += gary[i];
    }

    return (uint8_t)round(sum / k);
}

void BMP::kNeighborFilter(uint8_t k)
{
    if (k > 8) return;

    uint8_t mean;

    for (uint32_t r = 0; r < head.info.biHeight; ++r)
    {
        if (0 < r && r < head.info.biHeight-1) 
        {
            for (uint32_t c = 0; c < head.info.biWidth; ++c)
            {
                if (0 < c && c < head.info.biWidth-1) 
                {
                    mean = countNeighborAverage(5, r, c);
                    printf("%3d,", mean);
                }
                else {
                    printf("%3d,", RGB_GARY(r, c));
                }
            }
        }
        else {
            for (uint32_t c = 0; c < head.info.biWidth; ++c)
            {
                printf("%3d,", RGB_GARY(r, c));
            }
        }
        printf("\n");
    }
}

uint8_t BMP::symmetryMean(uint32_t row, uint32_t col)
{
    const uint8_t NUM = 4;
    uint8_t gary_center, gary[NUM][2];

    gary_center = RGB_GARY(row, col); // 第一个点为中心
    gary[0][0] = RGB_GARY(row-1, col-1);
    gary[0][1] = RGB_GARY(row+1, col+1);

    gary[1][0] = RGB_GARY(row-1, col  );
    gary[1][1] = RGB_GARY(row+1, col  );

    gary[2][0] = RGB_GARY(row-1, col+1);
    gary[2][1] = RGB_GARY(row+1, col-1);

    gary[3][0] = RGB_GARY(row,   col-1);
    gary[3][1] = RGB_GARY(row,   col+1);

    /* 计算平均值 */
    double sum = 0;

    // printf("center: %3d  | ", gary_center);
    for (uint8_t i = 0; i < NUM; ++i) {
        if (_abs(gary[i][0] - gary_center) < _abs(gary[i][1] - gary_center)) {
            sum += gary[i][0];
            //printf("%3d, ", gary[i][0]);
        }
        else {
            sum += gary[i][1];
            //printf("%3d, ", gary[i][1]);
        }
    }

    return (uint8_t)round(sum / NUM);
}

void BMP::symmetryMeanFilter()
{
    uint8_t mean;

    for (uint32_t r = 0; r < head.info.biHeight; ++r)
    {
        if (0 < r && r < head.info.biHeight-1) 
        {
            for (uint32_t c = 0; c < head.info.biWidth; ++c)
            {
                if (0 < c && c < head.info.biWidth-1) 
                {
                    mean = symmetryMean(r, c);
                    printf("%3d,", mean);
                }
                else {
                    printf("%3d,", RGB_GARY(r, c));
                }
            }
        }
        else {
            for (uint32_t c = 0; c < head.info.biWidth; ++c)
            {
                printf("%3d,", RGB_GARY(r, c));
            }
        }
        printf("\n");
    }
}


uint8_t BMP::sigmaMean(uint32_t row, uint32_t col)
{
    double mean, variance;
    uint32_t sumGary = 0;
    double sumPoor2 = 0;

    for (int8_t r = -1; r <= 1; ++r)
    {
        for (int8_t c = -1; c <= 1; ++c)
        {
            sumGary += RGB_GARY(row+r, col+c);
        }
    }
    mean = (double)sumGary / (TEMP_AREA*TEMP_AREA);
    
    for (int8_t r = -1; r <= 1; ++r)
    {
        for (int8_t c = -1; c <= 1; ++c)
        {
            sumPoor2 += (RGB_GARY(row+r, col+c) - mean) 
                        * (RGB_GARY(row+r, col+c) - mean);
        }
    }
    variance = sumPoor2 / (TEMP_AREA*TEMP_AREA);
    double min = RGB_GARY(row, col) - 2*variance;
    double max = RGB_GARY(row, col) + 2*variance;

    /* 计算平均值 */
    uint8_t temp;
    double sum = 0;
    uint32_t len = 0;

    // printf("center: %3d | ", RGB_GARY(row, col));
    // printf("mean: %6.2f, ", mean);
    // printf("variance: %8.2f, ", variance);

    // printf("[%9.2f, %9.2f] ", min, max);
    for (int8_t r = -1; r <= 1; ++r)
    {
        for (int8_t c = -1; c <= 1; ++c)
        {
            temp = RGB_GARY(row+r, col+c);
            if (min < temp && temp < max)
            {
                // printf("%3d,", temp);
                sum += temp;
                len++;
            }
        }
    }
    return (uint8_t)round(sum / len);
}

void BMP::SigmaFilter()
{
    uint8_t mean;

    for (uint32_t r = 0; r < head.info.biHeight; ++r)
    {
        if (1-1 < r && r < head.info.biHeight-1) 
        {
            for (uint32_t c = 0; c < head.info.biWidth; ++c)
            {
                if (1-1 < c && c < head.info.biWidth-1) 
                {
                    mean = sigmaMean(r, c);
                    printf("%3d,", mean);
                    // printf("\n");
                }
                else {
                    printf("%3d,", RGB_GARY(r, c));
                }
            }
        }
        else {
            for (uint32_t c = 0; c < head.info.biWidth; ++c)
            {
                printf("%3d,", RGB_GARY(r, c));
            }
        }
        printf("\n");
    }
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
    if (rgb == NULL) { // 如果没有rgb图像信息，发送错误消息
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
    for (uint32_t iy = 0; iy < head.info.biHeight; ++iy) {
        fwrite((rgb + (head.info.biHeight-1 - iy) * head.info.biWidth), sizeof(RGBInfoNode), head.info.biWidth, fp);
        for (uint32_t ix = 0; ix < redundant; ++ix) {
            fputc(0x00, fp);
        }
    }
    fclose(fp);
    return true;
}
