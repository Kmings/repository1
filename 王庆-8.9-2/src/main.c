#include <stdio.h>
#include "main.h"
#include "tools.h"

int main() {
    // 输入原始矩阵
    Matrix original = inputMatrix();
    if (!original) return 1;

    // 计算转置矩阵
    Matrix transposed = transposeMatrix(original);
    if (!transposed) {
        freeMatrix(original);
        return 1;
    }

    // 计算矩阵乘积
    Matrix product = multiplyMatrix(original, transposed);
    if (!product) {
        freeMatrix(original);
        freeMatrix(transposed);
        return 1;
    }

    // 输出结果
    printf("\n原始矩阵:\n");
    printMatrix(original);

    printf("\n转置矩阵:\n");
    printMatrix(transposed);

    printf("\n矩阵与转置的乘积:\n");
    printMatrix(product);

    // 释放内存
    freeMatrix(original);
    freeMatrix(transposed);
    freeMatrix(product);

    return 0;
}