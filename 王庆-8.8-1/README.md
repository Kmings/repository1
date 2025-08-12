# Float与Char数组转换

### 方法一：使用memcpy函数
- **原理**：通过标准库函数`memcpy`进行内存块复制
- **优点**：安全可靠，不会触发严格别名规则问题
- **代码实现**：
```c
void method1_memcpy(char *buffer, float f1, float f2) {
    memcpy(buffer, &f1, sizeof(float));
    memcpy(buffer + sizeof(float), &f2, sizeof(float));
}

void method1_extract(char *buffer, float *f1, float *f2) {
    memcpy(f1, buffer, sizeof(float));
    memcpy(f2, buffer + sizeof(float), sizeof(float));
}
```

### 方法二：指针强制转换
- **原理**：直接通过指针类型转换访问内存
- **优点**：代码简洁，无需函数调用开销
- **缺点**：可能违反C语言严格别名规则
- **代码实现**：
```c
void method2_memcpy(char *buffer, float f1, float f2) {
    *(float*)buffer = f1;
    *(float*)(buffer + sizeof(float)) = f2;
}
```

### 方法三：联合体(Union)方式
- **原理**：利用联合体的内存共享特性进行类型转换
- **优点**：符合C语言标准，避免严格别名问题
- **代码实现**：
```c
union FloatCharUnion { float f; char c[sizeof(float)]; };
void method3_memcpy(char *buffer, float f1, float f2) {
    union FloatCharUnion u1 = {.f = f1};
    union FloatCharUnion u2 = {.f = f2};
    memcpy(buffer, u1.c, sizeof(float));
    memcpy(buffer + sizeof(float), u2.c, sizeof(float));
}
```

## 测试结果
三种方法的精度差异如下（测试值：3.14159f和2.71828f）：

| 方法 | 提取值1 | 提取值2 | 差异1 | 差异2 |
|------|---------|---------|-------|-------|
| 方法一 | 3.141590 | 2.718280 | 0.0000000000 | 0.0000000000 |
| 方法二 | 3.141590 | 2.718280 | 0.0000000000 | 0.0000000000 |
| 方法三 | 3.141590 | 2.718280 | 0.0000000000 | 0.0000000000 |

