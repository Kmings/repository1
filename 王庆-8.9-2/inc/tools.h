#ifndef TOOLS_H
#define TOOLS_H

#include <stdbool.h>
#include <stdint.h>

// 矩阵结构体定义
typedef struct MParam {
    uint8_t row;
    uint8_t column;
    float **data;
} *Matrix;

// 函数声明
Matrix createMatrix(uint8_t row, uint8_t column);
void freeMatrix(Matrix mat);
Matrix transposeMatrix(Matrix mat);
Matrix multiplyMatrix(Matrix a, Matrix b);
Matrix inputMatrix();
void printMatrix(Matrix mat);

#endif