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



Vector reverseVector(Vector* v1) {
    Vector v;
    v.x = -1 * v1->x;
    v.y = -1 * v1->y;
    v.z = -1 * v1->z;
    
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


//伴随矩阵方法
Mat3 inverseMat3(Mat3 *matrix)
{
    Mat3 mat;
    
    double (*m)[3] = matrix->m;
    
    double	t00 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
    double	t01 = m[1][0] * m[2][2] - m[2][0] * m[1][2];
    double	t02 = m[1][0] * m[2][1] - m[2][0] * m[1][1];
    double	t10 = m[0][1] * m[2][2] - m[2][1] * m[0][2];
    double	t11 = m[2][2] * m[0][0] - m[0][2] * m[2][0];
    double	t12 = m[0][0] * m[2][1] - m[2][0] * m[0][1];
    double	t20 = m[0][1] * m[1][2] - m[0][2] * m[1][1];
    double	t21 = m[0][0] * m[1][2] - m[1][0] * m[0][2];
    double	t22 = m[0][0] * m[1][1] - m[1][0] * m[0][1];
    
    double fDetInv = 1.0f / (m[0][0] * t00 - m[0][1] * t01 + m[0][2] * t02);
    
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
    double r = 0;
    
    return r;
}




// 对输入Matrix 有限制
Mat4 inverseMat4(Mat4 *a)
{
    Mat4 mat; // tmp
    
    
    double (*m)[4] = a->m;
    
    if (fabs(m[3][3] - 1.0f) > .001f)
        throw "";
    //    if (fabs(a->m[0][3]) > .001f ||
    //        fabs(m[1][3]) > .001f ||
    //        fabs(m[2][3]) > .001f)
    //        throw "";

    
    double fDetInv = 1.0f /
    (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
     m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
     m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
    
    mat.m[0][0] = fDetInv * (m[1][1] * m[2][2] - m[1][2] * m[2][1]);
    mat.m[0][1] = -fDetInv * (m[0][1] * m[2][2] - m[0][2] * m[2][1]);
    mat.m[0][2] = fDetInv * (m[0][1] * m[1][2] - m[0][2] * m[1][1]);
    mat.m[0][3] = 0.0f;
    
    mat.m[1][0] = -fDetInv * (m[1][0] * m[2][2] - m[1][2] * m[2][0]);
    mat.m[1][1] = fDetInv * (m[0][0] * m[2][2] - m[0][2] * m[2][0]);
    mat.m[1][2] = -fDetInv * (m[0][0] * m[1][2] - m[0][2] * m[1][0]);
    mat.m[1][3] = 0.0f;
    
    mat.m[2][0] = fDetInv * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    mat.m[2][1] = -fDetInv * (m[0][0] * m[2][1] - m[0][1] * m[2][0]);
    mat.m[2][2] = fDetInv * (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
    mat.m[2][3] = 0.0f;
    
    mat.m[3][0] = -(m[3][0] * m[0][0] +
                    m[3][1] * m[1][0] +
                    m[3][2] * m[2][0]);
    mat.m[3][1] = -(m[3][0] * m[0][1] +
                    m[3][1] * m[1][1] +
                    m[3][2] * m[2][1]);
    mat.m[3][2] = -(m[3][0] * m[0][2] +
                    m[3][1] * m[1][2] +
                    m[3][2] * m[2][2]);
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
    //std::cout << m3.m[0][0] << "\t"  << m3.m[0][1] << "\t"  << m3.m[0][2] << "\t"  << m3.m[1][0] << "\t"  << m3.m[1][1] << "\t"  << m3.m[1][2] << "\t"
        //<< m3.m[2][0] << "\t"  << m3.m[2][1] << "\t"  << m3.m[2][2] << std::endl;
    return true;
}
//
Mat4 multiplyMat4(Mat4* m1, Mat4* m2)
{
    Mat4 mat = {0};
    
    return mat;
}


// v 是横向的，需要转置
Vector4 mulMat4_1(Mat4* mat1, Vector4* v) {
    Mat4_1 mat2 = transposeMat1_4(v);
    
    double (*m)[4] = mat1->m;
    double (*m2)[1] = mat2.m;
    
    double r0 = m[0][0] * m2[0][0] +    m[0][1] * m2[1][0] +   m[0][2] * m2[2][0] +  m[0][3] *   m2[3][0] ;
    double r1 = m[1][0] * m2[0][0] +    m[1][1] * m2[1][0] +   m[1][2] * m2[2][0] +  m[1][3] *   m2[3][0] ;
    
    double r2 = m[2][0] * m2[0][0] +    m[2][1] * m2[1][0] +   m[2][2] * m2[2][0] +  m[2][3] *   m2[3][0] ;
    double r3 = m[3][0] * m2[0][0] +    m[3][1] * m2[1][0] +   m[3][2] * m2[2][0] +  m[3][3] *   m2[3][0] ;
    
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


bool doesRaySphereIntersect(Ray* ray, Sphere* sphere)
{
    bool intersects = false;

    Vector centerIntersection = pointDifference(&ray->origin, &sphere->center);
    double cos_theta = scalarProduct(&ray->direction, &centerIntersection) / vectorLength(&ray->direction)
        / vectorLength(&centerIntersection);

    if (cos_theta > 0)
        intersects = false;
    else
        intersects = true;

    return intersects;
}























