#include <stdio.h>
#include <stdint.h>
#include "main.h"


int main() {
    uint32_t original_num = 0x12253647;
    int native_endian = is_little_endian();
    
    // 打印本机字节序
    printf("本机字节序: %s\n", native_endian ? "小端模式" : "大端模式");
    
    // 打印原始数字
    printf("原始数字: 0x%08X\n", original_num);
    
    // 小端模式下的字节表示（低字节在前）
    printf("小端模式字节序列: 0x%02X 0x%02X 0x%02X 0x%02X\n",
           (original_num & 0x000000FF),
           (original_num & 0x0000FF00) >> 8,
           (original_num & 0x00FF0000) >> 16,
           (original_num & 0xFF000000) >> 24);
    
    // 大端模式下的字节表示（高字节在前）
    printf("大端模式字节序列: 0x%02X 0x%02X 0x%02X 0x%02X\n",
           (original_num & 0xFF000000) >> 24,
           (original_num & 0x00FF0000) >> 16,
           (original_num & 0x0000FF00) >> 8,
           (original_num & 0x000000FF));
    
    // 转换后的数字
    uint32_t swapped_num = swap_endian(original_num);
    printf("大小端转换后: 0x%08X\n", swapped_num);
    
    return 0;
}

// 判断本机字节序（小端返回1，大端返回0）
int is_little_endian() {
    union {
        uint32_t i;
        uint8_t c[4];
    } u;
    u.i = 0x01;
    return (u.c[0] == 0x01);
}

// 大小端转换函数
uint32_t swap_endian(uint32_t num) {
    return ((num >> 24) & 0x000000FF) |
           ((num >> 8)  & 0x0000FF00) |
           ((num << 8)  & 0x00FF0000) |
           ((num << 24) & 0xFF000000);
}