/*
 * @Description: 
 * @FilePath: /MY/APP/Function/Math.h
 * @Author: Wei Zhou
 * @Github: https://github.com/zromyk
 * @Date: 1970-01-01 08:00:00
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2020-04-21 20:08:55
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */

#ifndef __MYMATH_H
#define __MYMATH_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>

#define MYMATH_TAN_MAP_RES          (0.003921569f)     /* (smallest non-zero value in table) */
#define MYMATH_RAD_PER_DEG          (0.017453293f)
#define MYMATH_TAN_MAP_SIZE         (256)
#define MYMATH_PI                   (3.1415926f)
#define MYMATH_PI_HALF              (1.5707963f)

#define MYMATH_ONE_PI               MYMATH_PI
#define MYMATH_TWO_PI               (2.0 * MYMATH_PI)
#define MYMATH_ANGLE_UNIT           (MYMATH_TWO_PI / 10.0)

#define myMath_pow(a)               ((a)*(a))
#define myMath_3_norm(x,y,z)        (myMath_sqrt(my_pow((x)) + myMath_pow((y)) + myMath_pow((z))))
#define myMath_2_norm(x,y)          (myMath_sqrt(my_pow((x)) + myMath_pow((y))))

#define _abs(x)         ((x) > 0 ? (x) : (-(x)))
#define _max(x, y)      (((x) > (y)) ? (x) : (y))    //maximum value
#define _min(x, y)      (((x) < (y)) ? (x) : (y))    //minimum value

#define _imax(x, y)     (((x) > (y)) ? (x) : (y))    //maximum value
#define _imin(x, y)     (((x) < (y)) ? (x) : (y))    //minimum value

#define _imid(a, b, c)  (b < c ? _limit(a, b, c) : _limit(a, c, b))//intermediate value

#define _iinc(i, a, b)  (i <= _imax(a, b) && i >= _imin(a, b)) //判断 i 在a,b之间
#define _iexc(i, a, b)  (i > _imax(a, b) || i < _imin(a, b))   //判断 i 不在a,b之间
#define _ninc(i, a, b)  (i >= a && i <= b)

#define _iiabs(x)       (((int)(x) > 0) ? (int)(x) : (-(int)(x)))  //求绝对值
#define _iidif(x, y)    _iiabs((int)(x - y))    //求差

#define _limit(a, b, c) (a > b ? (a < c ? a : c) : b) //注意：b < c

float   	myMath_sqrt(float number);
float   	myMath_fast_atan2(float y, float x);
float   	myMath_atan(float x, float y);
double  	myMath_mx_sin(double rad);
double  	myMath_sin(double rad);
float   	myMath_cos(double rad);
float   	myMath_linear_interpolation(float range[], float interpolation[], float in, uint8_t length); //range 必须从小到大

void		myMath_delay(long t);
uint8_t		myMath_iDigits(int num);
uint16_t	myMath_power(uint8_t m, uint8_t n);
float		myMath_normal(uint8_t x, float u, float o2);    //正态分布密度函数

float _ifabs(float number);

#endif

