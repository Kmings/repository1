#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

// 判断本机字节序
int is_little_endian();

// 大小端转换函数
uint32_t swap_endian(uint32_t num);

#endif // MAIN_H