#include "solve.h"
#include "vector_math.h"
#include <stdio.h>

#define PI 3.14159265358979323846
#define RAD2DEG(x) ((x) * 180.0f / PI)
#define DEG2RAD(x) ((x) * PI / 180.0f)

// vector3f half_operator = {0.5f, 0.5f, 0.5f};

vector3f A = {1.0f, 0.0f, 1.0f};
vector3f B = {1.0f, 1.0f, 1.0f};
vector3f C = {0.0f, 1.0f, 1.0f};
vector3f D = {0.0f, 0.0f, 1.0f};
vector3f A1 = {1.0f, 0.0f, 0.0f};
vector3f B1 = {1.0f, 1.0f, 0.0f};
vector3f C1 = {0.0f, 1.0f, 0.0f};
vector3f D1 = {0.0f, 0.0f, 0.0f};

int main(){
    printf("解：由题意得：\r\n");
    point3f_print(&A, "A");
    point3f_print(&B, "B");
    point3f_print(&C, "C");
    point3f_print(&D, "D");
    point3f_print(&A1, "A1");
    point3f_print(&B1, "B1");
    point3f_print(&C1, "C1");
    point3f_print(&D1, "D1");
    printf("    (1)不难算出：\r\n");
    vector3f AB = vector3f_subtract(&B, &A);
    vector3f_print(&AB, "AB");
    vector3f A1B1 = vector3f_subtract(&B1, &A1);
    vector3f_print(&A1B1, "A1B1");
    vector3f AC1 = vector3f_subtract(&C1, &A);
    vector3f_print(&AC1, "AC1");
    // vector3f cross_product = vector3f_cross(&AB, &AC1);
    // printf("    则点B到直线AC1的距离为:%f\r\n", vector3f_magnitude(&cross_product) / vector3f_magnitude(&AC1));
    printf("    由向量点积公式可知：\r\n");
    printf("    AB·AC1 = |AB|*|AC1|*cos(theta)\r\n");
    printf("    其中，theta为向量AB与向量AC1的夹角。\r\n");
    double theta = acos(vector3f_dot(&AB, &AC1) / (vector3f_magnitude(&AB) * vector3f_magnitude(&AC1)));
    printf("    向量AB与向量AC1的夹角为：%f(rad)\r\n", theta);
    printf("    则点B到直线AC1的距离为:%f\r\n", vector3f_magnitude(&AB)*sin(theta));

    printf("    (2)由E，F分别是A1B1，AB的中点可知：\r\n");
    vector3f E = vector3f_create(1.0f, 0.5f, 0.0f);
    point3f_print(&E, "E");
    vector3f F = vector3f_create(1.0f, 0.5f, 1.0f);
    point3f_print(&F, "F");
    vector3f AE = vector3f_subtract(&E, &A);
    vector3f_print(&AE, "AE");
    vector3f FC = vector3f_subtract(&C, &A);
    vector3f_print(&FC, "FC");
    vector3f C1E = vector3f_subtract(&E, &C1);
    vector3f_print(&C1E, "C1E");
    printf("    注意到:不难发现两向量平行，而显然FC与C1E不重合，则FC平行于平面AC1E。\r\n");
    printf("    因此，连接EF，设向量EF与平面AC1E法向量的夹角为alpha，则直线FC到平面AC1E的距离即|EF|*sin(alpha)。\r\n");
    printf("    则有：\r\n");
    vector3f EF = vector3f_subtract(&E, &F);
    vector3f_print(&EF, "EF");
    printf("    设平面AC1E的法向量为n，则\r\n");
    vector3f AC1E = vector3f_cross(&AC1, &AE);
    vector3f_print(&AC1E, "n");
    float alpha = acos(vector3f_dot(&EF, &AC1E) / (vector3f_magnitude(&EF) * vector3f_magnitude(&AC1E)));
    printf("    由向量点积公式可知：\r\n");
    printf("    EF·n = |EF|*|n|*cos(alpha)\r\n");
    printf("    因此，向量EF与平面AC1E法向量的夹角为：%f(rad)\r\n", alpha);
    printf("    因此，直线FC到平面AC1E的距离为：%f\r\n", vector3f_magnitude(&EF)*cos(alpha));
    
    return 0;
}
