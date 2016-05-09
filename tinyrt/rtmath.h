#ifndef __tinyrt__rtmath__
#define __tinyrt__rtmath__

#include "definition.h"
#include <math.h>

#include <stdio.h>


//bool inverseMat2_2(Mat2* mat1, Mat2* m2);


//bool transposeMat2_2();
Mat3 transposeMat3_3(Mat3* mat );
Mat4 transposeMat4_4(Mat4* mat );
Mat4_1 transposeMat1_4(Vector4* v);

Point transformPoint(Point p, Vector* v);

// double vectorLength(Vector v) ;
inline double vectorLength(Vector* v) { return sqrt(v->x*v->x + v->y*v->y + v->z*v->z); };
//Vector vectorAdd(Vector* v1, Vector* v2); // 
inline Vector vectorAdd(Vector* v1, Vector* v2) {
    return { v1->x + v2->x, v1->y + v2->y, v1->z + v2->z };
}; // 

Vector reverseVector(Vector* v1) ;   // 翻转向量
//Vector pointDifference(Point* p1, Point* p2); // vecotr p2p1
inline Vector pointDifference(Point* p1, Point* p2) {
    return  { p1->x - p2->x, p1->y - p2->y, p1->z - p2->z };
};
//Point pointAdd(Point *p1, Vector *v);
// 某点经过向量的transform，到达新的点，其实应该起名 transformPoint(Point, Vector)
inline Point pointAdd(Point* p1, Vector* v) {
    return { p1->x + v->x, p1->y + v->y, p1->z + v->z }; 
};

//double scalarProduct(Vector* v1, Vector* v2) ;  // 点乘
inline double scalarProduct(Vector* v1, Vector* v2){
    return v1->x*v2->x + v1->y*v2->y + v1->z*v2->z; 
};
//Vector crossVector(Vector* v1, Vector* v2) ;
// 需要注意v1 为x， v2为y，顺序不能改变， 否则构造出来 -z   x * y =z
// z * x = y
inline Vector crossVector(Vector* v1, Vector* v2){
    return { v1->y * v2->z - v2->y * v1->z,
            v1->z * v2->x - v2->z * v1->x,
            v1->x*v2->y - v1->y * v2->x };
};
void  normalizeColor(Color* color);
//Vector scalarVector(Vector* v, double scale);
inline Vector scalarVector(Vector* v, double scale) {
    Vector v1;
    v1.x = v->x * scale;
    v1.y = v->y * scale;
    v1.z = v->z * scale;
        
    return v1;
};
//Vector normalize(Vector* v);
inline Vector normalize(Vector* v){
    Vector v1;
    double l = vectorLength(v);
    v1.x = v->x / l;
    v1.y = v->y / l;
    v1.z = v->z / l;
    return  v1;
};

Mat3 inverseMat3(Mat3 *matrix);
Mat3 multiplyMat3(Mat3* m1, Mat3* m2);
Mat4 inverseMat4(Mat4 *a);
Mat4 multiplyMat4(Mat4* m1, Mat4* m2);
Vector4 mulMat4_1(Mat4* mat1, Vector4* v) ;

bool testMultiplyMat3();

Vector localToWorld(Vector* v, Vector* xPrime, Vector* yPrime, Vector* eye);

bool doesRaySphereIntersect(Ray* ray, Sphere* sphere);

Vector getReflect(Vector* n, Vector* v);


#endif /* defined(__tinyrt__rtmath__) */
