#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tools.h"

// 创建矩阵
Matrix createMatrix(uint8_t row, uint8_t column) {
    Matrix mat = (Matrix)malloc(sizeof(struct MParam));
    if (!mat) return NULL;

    mat->row = row;
    mat->column = column;
    mat->data = (float **)malloc(row * sizeof(float *));
    if (!mat->data) {
        free(mat);
        return NULL;
    }

    for (uint8_t i = 0; i < row; i++) {
        mat->data[i] = (float *)calloc(column, sizeof(float));
        if (!mat->data[i]) {
            // 释放已分配的内存
            for (uint8_t j = 0; j < i; j++) free(mat->data[j]);
            free(mat->data);
            free(mat);
            return NULL;
        }
    }
    return mat;
}

// 释放矩阵内存
void freeMatrix(Matrix mat) {
    if (!mat) return;

    for (uint8_t i = 0; i < mat->row; i++) {
        free(mat->data[i]);
    }
    free(mat->data);
    free(mat);
}

// 矩阵转置
Matrix transposeMatrix(Matrix mat) {
    if (!mat) return NULL;

    Matrix transposed = createMatrix(mat->column, mat->row);
    if (!transposed) return NULL;

    for (uint8_t i = 0; i < mat->row; i++) {
        for (uint8_t j = 0; j < mat->column; j++) {
            transposed->data[j][i] = mat->data[i][j];
        }
    }
    return transposed;
}

// 矩阵乘法
Matrix multiplyMatrix(Matrix a, Matrix b) {
    if (!a || !b || a->column != b->row) return NULL;

    Matrix result = createMatrix(a->row, b->column);
    if (!result) return NULL;

    for (uint8_t i = 0; i < a->row; i++) {
        for (uint8_t j = 0; j < b->column; j++) {
            result->data[i][j] = 0;
            for (uint8_t k = 0; k < a->column; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    return result;
}

// 从键盘输入矩阵
Matrix inputMatrix() {
    uint8_t row, column;
    printf("请输入矩阵的行数和列数: ");
    scanf("%hhu %hhu", &row, &column);

    Matrix mat = createMatrix(row, column);
    if (!mat) {
        printf("矩阵创建失败!");
        return NULL;
    }

    printf("请输入矩阵元素(%dx%d):\n", row, column);
    for (uint8_t i = 0; i < row; i++) {
        for (uint8_t j = 0; j < column; j++) {
            scanf("%f", &mat->data[i][j]);
        }
    }
    return mat;
}

// 打印矩阵
void printMatrix(Matrix mat) {
    if (!mat) return;

    for (uint8_t i = 0; i < mat->row; i++) {
        for (uint8_t j = 0; j < mat->column; j++) {
            printf("%.2f\t", mat->data[i][j]);
        }
        printf("\n");
    }
}