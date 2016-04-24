//
//  rtmath.h
//  tinyrt
//
//  Created by Claude Q. on 4/23/16.
//  Copyright (c) 2016 A. S. All rights reserved.
//

#ifndef __tinyrt__rtmath__
#define __tinyrt__rtmath__

#include "definition.h"


#include <stdio.h>


//bool inverseMat2_2(Mat2* mat1, Mat2* m2);


//bool transposeMat2_2();
Mat3 transposeMat3_3(Mat3* mat );
Mat4 transposeMat4_4(Mat4* mat );
Mat4_1 transposeMat1_4(Vector4* v);


double vectorLength(Vector v) ;
Vector vectorAdd(Vector v1, Vector v2);

Vector reverseVector(Vector v1) ;
Vector pointDifference(Point p1, Point p2);
Point pointAdd(Point p1, Vector v);
double scalarProduct(Vector* v1, Vector* v2) ;
Vector crossVector(Vector* v1, Vector* v2) ;
void  normalizeColor(Color* color);
Vector scalarVector(Vector* v, double scale);
Vector normalize(Vector* v);
Mat3 inverseMat3(Mat3 *matrix);
Mat4 inverseMat4(Mat4 *a);

Mat3 multiplyMat3(Mat3* m1, Mat3* m2);

bool testMultiplyMat3();
Mat4 multiplyMat4(Mat4* m1, Mat4* m2);
Vector4 mulMat4_1(Mat4* mat1, Vector4* v) ;

Vector localToWorld(Vector* v, Vector* xPrime, Vector* yPrime, Vector* eye);


#endif /* defined(__tinyrt__rtmath__) */
