#include "solve.h"

int main() {
    // 初始化随机数种子，使用当前时间确保每次运行结果不同
    srand(time(NULL));
    
    uint32_t result = 0;  // 存储最终结果的32位无符号整数
    
    for (int i = 0; i < 32; i++) {
        // 生成0~100之间的随机整数
        int random_num = rand() % 101;
        
        // 如果随机数大于50，设置第(i+1)位（1-based）为1
        // 注意：位操作中使用0-based索引，因此左移i位对应第(i+1)位
        if (random_num > 50) {
            result |= (1 << i); //将1左移i位，再与32位整数进行或运算
        }
    }
    
    printf("生成的32位整数结果（十进制）: %d\n", result);

    printf("生成的32位整数结果（二进制）: 0b");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (result >> i) & 1);
    }
    printf("\n");

    printf("生成的32位整数结果（十六进制）: 0x%08X\n", result);

    return 0;
}