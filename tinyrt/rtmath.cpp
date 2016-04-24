//
//  rtmath.cpp
//  tinyrt
//
//  Created by Claude Q. on 4/23/16.
//  Copyright (c) 2016 A. S. All rights reserved.
//

#include "rtmath.h"
#include "definition.h"
#include <math.h>


Mat4_1 transposeMat1_4(Vector4* v){
    Mat4_1 mat;
    mat.m[0][0] = v->x;
    mat.m[1][0] = v->y;
    mat.m[2][0] = v->z;
    mat.m[3][0] = v->w;
    
    return mat;
}




double vectorLength(Vector v) {
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vector vectorAdd(Vector v1, Vector v2){
    Vector v = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
    return v;
}

Vector reverseVector(Vector v1) {
    Vector v;
    v.x = -1 * v1.x;
    v.y = -1 * v1.y;
    v.z = -1 * v1.z;
    
    return v;
}


Vector pointDifference(Point p1, Point p2) {
    Vector v = { p1.x - p2.x, p1.y - p2.y, p1.z - p2.z };
    return v;
}

// 某点经过向量的transform，到达新的点，其实应该起名 transformPoint(Point, Vector)
Point pointAdd(Point p1, Vector v) {
    Vector v1 = { p1.x + v.x, p1.y + v.y, p1.z + v.z };
    return v1;
}

double scalarProduct(Vector* v1, Vector* v2) {
    return v1->x*v2->x + v1->y*v2->y + v1->z*v2->z;
}

Vector crossVector(Vector* v1, Vector* v2) {
    Vector v = { v1->y * v2->z - v2->y * v1->z,
                v1->z * v2->x - v2->z * v1->x,
                v1->x*v2->y - v1->y * v2->x };
    
    return v;
}


void  normalizeColor(Color* color) {
    double factor = 255 / fmax(color->x, fmax(color->y, color->z));
    if (factor < 1) {
        color->x = color->x*factor;
        color->y = color->y*factor;
        color->z = color->z*factor;
    }
}

Vector scalarVector(Vector* v, double scale)
{
    Vector v1;
    v1.x = v->x * scale;
    v1.y = v->y * scale;
    v1.z = v->z * scale;
    
    return v1;
}

Vector normalize(Vector* v)
{
    Vector v1;
    double l = vectorLength(*v);
    v1.x = v->x / l;
    v1.y = v->y / l;
    v1.z = v->z / l;
    return  v1;
}

//伴随矩阵方法
Mat3 inverseMat3(Mat3 *matrix)
{
    Mat3 mat;
    
    double	t00 = matrix->m[1][1] * matrix->m[2][2] - matrix->m[2][1] * matrix->m[1][2];
    double	t01 = matrix->m[1][0] * matrix->m[2][2] - matrix->m[2][0] * matrix->m[1][2];
    double	t02 = matrix->m[1][0] * matrix->m[2][1] - matrix->m[2][0] * matrix->m[1][1];
    double	t10 = matrix->m[0][1] * matrix->m[2][2] - matrix->m[2][1] * matrix->m[0][2];
    double	t11 = matrix->m[2][2] * matrix->m[0][0] - matrix->m[0][2] * matrix->m[2][0];
    double	t12 = matrix->m[0][0] * matrix->m[2][1] - matrix->m[2][0] * matrix->m[0][1];
    double	t20 = matrix->m[0][1] * matrix->m[1][2] - matrix->m[0][2] * matrix->m[1][1];
    double	t21 = matrix->m[0][0] * matrix->m[1][2] - matrix->m[1][0] * matrix->m[0][2];
    double	t22 = matrix->m[0][0] * matrix->m[1][1] - matrix->m[1][0] * matrix->m[0][1];
    
    double fDetInv = 1.0f / (matrix->m[0][0] * t00 - matrix->m[0][1] * t01 + matrix->m[0][2] * t02);
    
    mat.m[0][0] = fDetInv * t00;
    mat.m[0][1] = fDetInv * t10;
    mat.m[0][2] = fDetInv * t20;
    mat.m[1][0] = fDetInv * t01;
    mat.m[1][1] = fDetInv * t11;
    mat.m[1][2] = fDetInv * t21;
    mat.m[2][0] = fDetInv * t02;
    mat.m[2][1] = fDetInv * t12;
    mat.m[2][2] = fDetInv * t22;
    
    return mat;
}


double determinant(Mat3* mat)
{
    double r ;
    
    return r;
}




// 对输入Matrix 有限制
Mat4 inverseMat4(Mat4 *a)
{
    Mat4 mat; // tmp
    
    if (fabs(a->m[3][3] - 1.0f) > .001f)
        throw "";
    //    if (fabs(a->m[0][3]) > .001f ||
    //        fabs(a->m[1][3]) > .001f ||
    //        fabs(a->m[2][3]) > .001f)
    //        throw "";
    
    double fDetInv = 1.0f /
    (a->m[0][0] * (a->m[1][1] * a->m[2][2] - a->m[1][2] * a->m[2][1]) -
     a->m[0][1] * (a->m[1][0] * a->m[2][2] - a->m[1][2] * a->m[2][0]) +
     a->m[0][2] * (a->m[1][0] * a->m[2][1] - a->m[1][1] * a->m[2][0]));
    
    mat.m[0][0] = fDetInv * (a->m[1][1] * a->m[2][2] - a->m[1][2] * a->m[2][1]);
    mat.m[0][1] = -fDetInv * (a->m[0][1] * a->m[2][2] - a->m[0][2] * a->m[2][1]);
    mat.m[0][2] = fDetInv * (a->m[0][1] * a->m[1][2] - a->m[0][2] * a->m[1][1]);
    mat.m[0][3] = 0.0f;
    
    mat.m[1][0] = -fDetInv * (a->m[1][0] * a->m[2][2] - a->m[1][2] * a->m[2][0]);
    mat.m[1][1] = fDetInv * (a->m[0][0] * a->m[2][2] - a->m[0][2] * a->m[2][0]);
    mat.m[1][2] = -fDetInv * (a->m[0][0] * a->m[1][2] - a->m[0][2] * a->m[1][0]);
    mat.m[1][3] = 0.0f;
    
    mat.m[2][0] = fDetInv * (a->m[1][0] * a->m[2][1] - a->m[1][1] * a->m[2][0]);
    mat.m[2][1] = -fDetInv * (a->m[0][0] * a->m[2][1] - a->m[0][1] * a->m[2][0]);
    mat.m[2][2] = fDetInv * (a->m[0][0] * a->m[1][1] - a->m[0][1] * a->m[1][0]);
    mat.m[2][3] = 0.0f;
    
    mat.m[3][0] = -(a->m[3][0] * a->m[0][0] +
                    a->m[3][1] * a->m[1][0] +
                    a->m[3][2] * a->m[2][0]);
    mat.m[3][1] = -(a->m[3][0] * a->m[0][1] +
                    a->m[3][1] * a->m[1][1] +
                    a->m[3][2] * a->m[2][1]);
    mat.m[3][2] = -(a->m[3][0] * a->m[0][2] +
                    a->m[3][1] * a->m[1][2] +
                    a->m[3][2] * a->m[2][2]);
    mat.m[3][3] = 1.0f;
    
    return mat;
}

Mat3 multiplyMat3(Mat3* m1, Mat3* m2){
    double r0 =   m1->m[0][0] * m2->m[0][0] +  m1->m[0][1] * m2->m[1][0] +     m1->m[0][2] * m2->m[2][0];
    double r1 =   m1->m[0][0] * m2->m[0][1] +  m1->m[0][1] * m2->m[1][1] +     m1->m[0][2] * m2->m[2][1];
    double r2 =   m1->m[0][0] * m2->m[0][2] +  m1->m[0][1] * m2->m[1][2] +     m1->m[0][2] * m2->m[2][2];
    
    double r3 =   m1->m[1][0] * m2->m[0][0] +  m1->m[1][1] * m2->m[1][0] +     m1->m[1][2] * m2->m[2][0];
    double r4 =   m1->m[1][0] * m2->m[0][1] +  m1->m[1][1] * m2->m[1][1] +     m1->m[1][2] * m2->m[2][1];
    double r5 =   m1->m[1][0] * m2->m[0][2] +  m1->m[1][1] * m2->m[1][2] +     m1->m[1][2] * m2->m[2][2];
    
    double r6 =   m1->m[2][0] * m2->m[0][0] +  m1->m[2][1] * m2->m[1][0] +     m1->m[2][2] * m2->m[2][0];
    double r7 =   m1->m[2][0] * m2->m[0][1] +  m1->m[2][1] * m2->m[1][1] +     m1->m[2][2] * m2->m[2][1];
    double r8 =   m1->m[2][0] * m2->m[0][2] +  m1->m[2][1] * m2->m[1][2] +     m1->m[2][2] * m2->m[2][2];
    
    
    Mat3 m = {
        r0, r1, r2,
        r3,r4, r5,
        r6, r7, r8
    };
    
    return m;
}

bool testMultiplyMat3()
{
    Mat3 m1 = {1, 3, 2,     1, 0, 0,    1, 2, 2};
    Mat3 m2 = {0, 0, 2,     7, 5, 0,    2, 1, 1};
    
    Mat3 m3 = multiplyMat3(&m1, &m2);
    //std::cout << m3.m[0][0] << "\t"  << m3.m[0][1] << "\t"  << m3.m[0][2] << "\t"  << m3.m[1][0] << "\t"  << m3.m[1][1] << "\t"  << m3.m[1][2] << "\t"  << m3.m[2][0] << "\t"  << m3.m[2][1] << "\t"  << m3.m[2][2] << std::endl;
    //
    return true;
}
//
Mat4 multiplyMat4(Mat4* m1, Mat4* m2)
{
    Mat4 mat;
    
    return mat;
}


// v 是横向的，需要转置
Vector4 mulMat4_1(Mat4* mat1, Vector4* v) {
    Mat4_1 mat2 = transposeMat1_4(v);
    double r0 = mat1->m[0][0] * mat2.m[0][0] +    mat1->m[0][1] * mat2.m[1][0] +   mat1->m[0][2] * mat2.m[2][0] +  mat1->m[0][3] *   mat2.m[3][0] ;
    double r1 = mat1->m[1][0] * mat2.m[0][0] +    mat1->m[1][1] * mat2.m[1][0] +   mat1->m[1][2] * mat2.m[2][0] +  mat1->m[1][3] *   mat2.m[3][0] ;
    
    double r2 = mat1->m[2][0] * mat2.m[0][0] +    mat1->m[2][1] * mat2.m[1][0] +   mat1->m[2][2] * mat2.m[2][0] +  mat1->m[2][3] *   mat2.m[3][0] ;
    double r3 = mat1->m[3][0] * mat2.m[0][0] +    mat1->m[3][1] * mat2.m[1][0] +   mat1->m[3][2] * mat2.m[2][0] +  mat1->m[3][3] *   mat2.m[3][0] ;
    
    Vector4 vec = {r0, r1, r2, r3};
    
    return vec;
}


// lookat为x', up 为 y',
Vector localToWorld(Vector* v, Vector* xPrime, Vector* yPrime, Vector* eye)
{
    Vector c = crossVector(xPrime, yPrime);
    Vector zPrime = normalize(&c);
    Mat3 pM = {  xPrime->x, yPrime->x, zPrime.x , xPrime->y, yPrime->y, zPrime.y ,  xPrime->z, yPrime->z, zPrime.z  };
    pM = inverseMat3(&pM);
    
    Mat3 qM = {  1, 0, 0  , 0, 1, 0 , 0, 0, 1 };
    
    Mat3 M = multiplyMat3(&qM, &pM);
    
    Mat4  world = { M.m[0][0], M.m[0][1], M.m[0][2], eye->x ,
                    M.m[1][0], M.m[1][1], M.m[1][2], eye->y ,
                    M.m[2][0], M.m[2][1], M.m[2][2], eye->z ,
                    0,          0,          0,          1  };
    //Mat4 world_inverted = inverseMat4(&world_inverted);
 
    
    Vector4 tv = {v->x, v->y, v->z, 1};
    Vector4 realP = mulMat4_1(&world, &tv);
    
    Vector upPrime = { realP.x, realP.y, realP.z };
    
    
    return upPrime;
}


























