#include "method3.h"
#include "main.h"

// 使用联合体进行类型转换
union FloatCharUnion {
    float f;
    char c[sizeof(float)];
};

void method3_memcpy(char *buffer, float f1, float f2) {
    union FloatCharUnion u1, u2;
    u1.f = f1;
    u2.f = f2;
    
    // 复制字节到缓冲区
    for(int i = 0; i < sizeof(float); i++) {
        buffer[i] = u1.c[i];
        buffer[i + sizeof(float)] = u2.c[i];
    }
}

void method3_extract(char *buffer, float *f1, float *f2) {
    union FloatCharUnion u1, u2;
    
    // 从缓冲区复制字节
    for(int i = 0; i < sizeof(float); i++) {
        u1.c[i] = buffer[i];
        u2.c[i] = buffer[i + sizeof(float)];
    }
    
    *f1 = u1.f;
    *f2 = u2.f;
}