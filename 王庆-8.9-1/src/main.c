#include <stdio.h>
#include "main.h"
#include "tools.h"

int main() {
    float originalA = 59.622f;
    float originalB = 92.6327f;
    char floatBuffer[4];  // 严格4字节数组

    storeTwoFloats(floatBuffer, originalA, originalB);
    float extractedA, extractedB;
    extractTwoFloats(floatBuffer, &extractedA, &extractedB);

    printf("原始值: %.4f, %.4f\n", originalA, originalB);
    printf("提取值: %.4f, %.4f\n", extractedA, extractedB);
    printf("误差: %.4e, %.4e\n", originalA - extractedA, originalB - extractedB);
    
    return 0;
}