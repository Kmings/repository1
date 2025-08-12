#include <string.h>
#include "tools.h"

/**
 * @brief 将float转换为半精度浮点数
 * 
 * @param f 输入的float值
 * @return HalfFloat 转换后的半精度浮点数
 */
HalfFloat float2Half(float f) {
    uint32_t fBits = *(const uint32_t*)&f;
    HalfFloat result;
    // printf("fBits = %d\n", fBits);
    uint16_t sign = (fBits >> 16) & 0x8000;
    // printf("sign = %d\n", sign);
    int32_t exp = ((fBits >> 23) & 0xFF) - 127 + 15;
    // printf("exp = %d\n", exp);
    if (exp > 31) exp = 31;
    else if (exp < 0) exp = 0;
    uint16_t mantissa = (fBits >> 13) & 0x3FF;
    
    result.value = sign | ((uint16_t)exp << 10) | mantissa;
    return result;
}

/**
 * @brief 将半精度浮点数转换为float
 * 
 * @param h 输入的半精度浮点数
 * @param result 输出的float值指针
 * @return true 转换成功
 * @return false 转换失败（result为NULL）
 */
bool half2Float(HalfFloat h, float* result) {
    if (result == NULL) return false;
    
    uint32_t fBits = 0;
    // 符号位（1位）
    fBits |= (uint32_t)(h.value >> 15) << 31;
    
    // 指数位（5位转8位）
    int32_t exp = (h.value >> 10) & 0x1F;
    if (exp != 0) {
        exp += 127 - 15;
        fBits |= (uint32_t)exp << 23;
    }
    
    // 尾数位（10位转23位）
    fBits |= (uint32_t)(h.value & 0x3FF) << 13;
    
    *result = *(float*)&fBits;
    return true;
}

/**
 * @brief 将两个float值存储到4字节数组中
 * 
 * @param arr 输出的4字节数组指针
 * @param a 第一个float值
 * @param b 第二个float值
 */
void storeTwoFloats(char* arr, float a, float b) {
    if (arr == NULL) return;
    HalfFloat h1 = float2Half(a);
    HalfFloat h2 = float2Half(b);
    memcpy(arr, &h1.value, 2);
    memcpy(arr + 2, &h2.value, 2);
}

/**
 * @brief 从4字节数组中提取两个float值
 * 
 * @param arr 输入的4字节数组指针
 * @param a 输出的第一个float值指针
 * @param b 输出的第二个float值指针
 * @return true 提取成功
 * @return false 提取失败（arr或a或b为NULL）
 */
bool extractTwoFloats(char* arr, float* a, float* b) {
    if (arr == NULL || a == NULL || b == NULL) return false;
    HalfFloat h1, h2;
    memcpy(&h1.value, arr, 2);
    memcpy(&h2.value, arr + 2, 2);
    return half2Float(h1, a) && half2Float(h2, b);
}
