#include "method1.h"
#include "main.h"

void method1_memcpy(char *buffer, float f1, float f2) {
    memcpy(buffer, &f1, sizeof(float));
    memcpy(buffer + sizeof(float), &f2, sizeof(float));
}

void method1_extract(char *buffer, float *f1, float *f2) {
    memcpy(f1, buffer, sizeof(float));
    memcpy(f2, buffer + sizeof(float), sizeof(float));
}
