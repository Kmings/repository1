#include "main.h"
#include "method1.h"
#include "method2.h"
#include "method3.h"

int main() {
    char buffer[8] = {0};
    float original[2] = {3.14159f, 2.71828f};
    float extracted[2] = {0};
    double diff1, diff2;

    // 测试方法一
    method1_memcpy(buffer, original[0], original[1]);
    method1_extract(buffer, &extracted[0], &extracted[1]);
    diff1 = fabs(original[0] - extracted[0]);
    diff2 = fabs(original[1] - extracted[1]);
    printf("Method1: Extracted[0] = %f, Extracted[1] = %f, Diff = %.10f, %.10f\n", extracted[0], extracted[1], diff1, diff2);

    // 测试方法二
    method2_memcpy(buffer, original[0], original[1]);
    method2_extract(buffer, &extracted[0], &extracted[1]);
    diff1 = fabs(original[0] - extracted[0]);
    diff2 = fabs(original[1] - extracted[1]);
    printf("Method2: Extracted[0] = %f, Extracted[1] = %f, Diff = %.10f, %.10f\n", extracted[0], extracted[1], diff1, diff2);

    // 测试方法三
    method3_memcpy(buffer, original[0], original[1]);
    method3_extract(buffer, &extracted[0], &extracted[1]);
    diff1 = fabs(original[0] - extracted[0]);
    diff2 = fabs(original[1] - extracted[1]);
    printf("Method3: Extracted[0] = %f, Extracted[1] = %f, Diff = %.10f, %.10f\n", extracted[0], extracted[1], diff1, diff2);

    return 0;
}