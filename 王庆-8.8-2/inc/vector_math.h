#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * 三维向量结构体定义
 * 包含x、y、z三个float类型分量
 */
typedef struct {
    float x;
    float y;
    float z;
} vector3f;

/**
 * 创建新的三维向量
 * @param x X轴分量
 * @param y Y轴分量
 * @param z Z轴分量
 * @return 初始化后的vector3f结构体
 */
vector3f vector3f_create(float x, float y, float z);

/**
 * 向量加法
 * @param a 第一个向量
 * @param b 第二个向量
 * @return 两个向量之和 (a + b)
 */
vector3f vector3f_add(const vector3f* a, const vector3f* b);

/**
 * 向量减法
 * @param a 被减向量
 * @param b 减向量
 * @return 两个向量之差 (a - b)
 */
vector3f vector3f_subtract(const vector3f* a, const vector3f* b);

/**
 * 向量点积
 * @param a 第一个向量
 * @param b 第二个向量
 * @return 点积结果 (a·b)
 */
float vector3f_dot(const vector3f* a, const vector3f* b);

/**
 * 向量叉积
 * @param a 第一个向量
 * @param b 第二个向量
 * @return 叉积结果 (a × b)
 */
vector3f vector3f_cross(const vector3f* a, const vector3f* b);

/**
 * 向量求模
 * @param v 输入向量
 * @return 向量的模长 |v|
 */
float vector3f_magnitude(const vector3f* v);

/**
 * 向量单位化
 * @param v 输入向量
 * @return 单位化后的向量 (v/|v|)
 * @note 若输入向量模长为0，返回零向量
 */
vector3f vector3f_normalize(const vector3f* v);

/**
 * 向量数乘
 * @param v 输入向量
 * @param scalar 标量值
 * @return 数乘结果 (v * scalar)
 */
vector3f vector3f_multiply_scalar(const vector3f* v, float scalar);

/**
 * 打印向量信息
 * @param v 输入向量
 * @param name 向量名称(用于输出标识)
 */
void vector3f_print(const vector3f* v, const char* name);

/**
 * 打印点信息
 * @param v 输入点
 * @param name 点名称(用于输出标识)
 */
void point3f_print(const vector3f* v, const char* name);
#endif