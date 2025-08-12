#ifndef TOOLS_H
#define TOOLS_H

#include <stdbool.h>
#include <stdint.h>

// 大驼峰结构体定义
typedef struct {
    uint16_t value; // 使用标准uint16_t类型
} HalfFloat;

// 小驼峰函数声明
HalfFloat float2Half(float f);
bool half2Float(HalfFloat h, float* result);
void storeTwoFloats(char* arr, float a, float b);
bool extractTwoFloats(char* arr, float* a, float* b);

#endif