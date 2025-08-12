#include "method2.h"
#include "main.h"

void method2_memcpy(char *buffer, float f1, float f2) {
    // 使用指针强制转换直接赋值
    *(float*)buffer = f1;
    *(float*)(buffer + sizeof(float)) = f2;
}

void method2_extract(char *buffer, float *f1, float *f2) {
    // 使用指针强制转换提取数据
    *f1 = *(float*)buffer;
    *f2 = *(float*)(buffer + sizeof(float));
}
