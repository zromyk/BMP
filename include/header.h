/*
 * @Description:
 * @FilePath: /Area-cpp/src/header.h
 * @Author: Wei Zhou
 * @Github: https://github.com/muyi2414
 * @Date: 2019-09-20 19:47:33
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2020-02-28 20:14:35
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */

#ifndef HEADER_H
#define HEADER_H

#ifdef _WIN32
    #define CLEAR "cls"
    //define something for Windows (32-bit and 64-bit, this part is common)
    #ifdef _WIN64
    //define something for Windows (64-bit only)
    #else
    //define something for Windows (32-bit only)
    #endif
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
        // iOS Simulator
    #elif TARGET_OS_IPHONE
        // iOS device
    #elif TARGET_OS_MAC
        #define CLEAR "clear"
        // Other kinds of Mac OS
    #else
    #error "Unknown Apple platform"
    #endif
#elif __ANDROID__
    // android
#elif __linux__
    #define CLEAR "clear"
    // linux
#elif __unix__ // all unices not caught above
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#else
#error "Unknown compiler"
#endif

#endif // HEADER_H
