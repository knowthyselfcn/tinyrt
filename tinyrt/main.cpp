#include "definition.h"

#include "bmp.h"


#include <math.h>
#include <float.h>

#include <iostream>

Point light = { 0, 10, 0 };		// 点光源的位置
Point center = { 0, 0, 0 };     // 坐标系原点
Point eye = {0, 20, 20};
Vector lookAtDir = {0, -1, -1};  //眼睛向前的方向
Vector up = {0, 0, -1};  // 头顶的方向, 局部坐标系， 
double distance = 480; //  眼睛与视窗的距离
const static int width = 640 * 2;
const static int height = 480 * 2;


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
    Vector v = { v1->y * v2->z - v2->y * v1->z, v1->z * v2->x - v2->z * v1->x, v2->x*v2->y - v1->y * v2->x };
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

Mat3 inverseMat3(Mat3 *matrix)
{
    Mat3 mat;

    //double* m = &(mat.m);
    
    double	t00 = matrix->m[1][1] * matrix->m[2][2] - matrix->m[2][1] * matrix->m[1][2];
    double	t10 = matrix->m[2][1] * matrix->m[0][2] - matrix->m[0][1] * matrix->m[2][2];
    double	t20 = matrix->m[0][1] * matrix->m[1][2] - matrix->m[1][1] * matrix->m[0][2];
    double	t01 = matrix->m[1][2] * matrix->m[2][0] - matrix->m[2][2] * matrix->m[1][0];
    double	t11 = matrix->m[2][2] * matrix->m[0][0] - matrix->m[0][2] * matrix->m[2][0];
    double	t21 = matrix->m[0][2] * matrix->m[1][0] - matrix->m[1][2] * matrix->m[0][0];
    double	t02 = matrix->m[1][0] * matrix->m[2][1] - matrix->m[2][0] * matrix->m[1][1];
    double	t12 = matrix->m[2][0] * matrix->m[0][1] - matrix->m[0][0] * matrix->m[2][1];
    double	t22 = matrix->m[0][0] * matrix->m[1][1] - matrix->m[1][0] * matrix->m[0][1];

    double fDetInv = 1.0f / (matrix->m[0][0] * t00 + matrix->m[0][1] * t01 + matrix->m[0][2] * t02);

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

// 对输入Matrix 有限制
Mat4 inverseMat4(Mat4 *a)
{
    Mat4 mat; // tmp 

    if (fabs(a->m[3][3] - 1.0f) > .001f)
        throw "";
    if (fabs(a->m[0][3]) > .001f ||
        fabs(a->m[1][3]) > .001f ||
        fabs(a->m[2][3]) > .001f)
        throw "";

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

//Mat3 multiplyMat3(Mat3* m1, Mat3* m2){
//    Mat3 m;
//
//    return m;
//}
//
//Mat4 multiplyMat4(Mat4* m1, Mat4* m2)
//{
//
//}
//
//Vector4 mulMat4_1(Mat4* mat, Vector4* v) {
//
//}


bool isLineParallel(Ray* r1, Ray* r2)
{
    return false;
}

// lookat为x', up 为 y', 
Vector localToWorld(Point* p, Vector* xPrime, Vector* yPrime)
{
    Vector zPrime = normalize(&crossVector(xPrime, yPrime));
    Mat3 pM = {  xPrime->x, yPrime->x, zPrime.x , xPrime->y, yPrime->y, zPrime.y ,  xPrime->z, yPrime->z, zPrime.z  };
    pM = inverseMat3(&pM);

    Mat3 qM = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };

    Mat3 M = multiplyMat3(&qM, &pM);

    Mat4 world_inverted = { { M.m[0][0], M.m[0][1], M.m[0][2], eye.x },
                    { M.m[1][0], M.m[1][1], M.m[1][2], eye.y },
                    { M.m[2][0], M.m[2][1], M.m[2][2], eye.y },
                    { 0, 0, 0, 1 } };
    Mat4 world = inverseMat4(&world_inverted);
    Vector4 v = {p->x, p->y, p->z, 1};
    Vector4 realP = mulMat4_1(&world, &v);

    Vector upPrime = { realP.x, realP.y, realP.z };


    return upPrime;
}


Ray getRay(int w, int h) {
	Ray r;
    r.origin = eye;         // 设定眼睛的位置

    Vector viewRight2Left = reverseVector(  crossVector(&lookAtDir, &up) );

    Vector rayDir = vectorAdd(scalarVector(&lookAtDir, distance),
        vectorAdd(scalarVector(&up, height), scalarVector(&viewRight2Left, width)));
    r.direction = rayDir;

	//r.direction.x = -width / 2 + w + 0.5;	// left to right
	//r.direction.y = height / 2 - h + 0.5;	// top to down
	//r.direction.z = -960;
	return r;
}



Color traceRay(Ray* ray, Plane* plane, Sphere* sphere)
{
    Color color = { 0, 0, 0 };
    // 针对每一条视线，都需要判断首次与哪个物体相交： 判断交点到眼睛的距离即可

    return color;
}

// 计算直线与平面的交点
/*
设眼睛位置E，视线向量v，平面经过点p，其法线n与E同侧， 视线与平面交点Z， EP 与 -n 夹角theta， E 到平面距离h, EZ 与 -n 夹角 beta,
| Z - E | = l =  h /  cos(beta) = EP * cos(theta) / (dot(v,n) / length(v) / length(n) )
*/
Color traceRay_Plane(Ray* ray, Plane* plane) {
	Color color = { 0, 0, 0 };

    Vector reverseN = reverseVector(plane->normal);
    double cos_beta = scalarProduct(&ray->direction, &reverseN) / vectorLength(ray->direction) / vectorLength(reverseN);

    //std::cout << cos_beta << std::endl;
    if (cos_beta < 0) {
        // no intersect  展示天空
        color = { 255, 255, 255 };
    }
    else {
        Vector ep = pointDifference(plane->p, ray->origin);
        double cos_theta = scalarProduct(&ep, &reverseN) / vectorLength(ep) / vectorLength(reverseN);
        double h = vectorLength(ep) * cos_theta;
        
        Vector normaledRay = normalize(&ray->direction);
        Vector ez = scalarVector(&normaledRay, (h / cos_beta));
       
        Point z = pointAdd(ray->origin, ez);

        Intersection intersect = { z, 0, plane };

        Vector directionToLight = pointDifference(light, intersect.point);
        Ray lightRay = { intersect.point, directionToLight };

        double length = vectorLength(directionToLight);
        

        double scale = 1 / (length * length *0.001 + 0.003 * length +  1);

        color = intersect.plane->color;
        color.x *= scale;				// 模拟光源光衰减
        color.y *= scale;
        color.z *= scale;
    }

	return color;
}



int main(int argc, char* argv[]) {
	Plane basePlane = { 0, 1, 0,   0,0,0,  255, 0, 0 };    

    Sphere sphere = {0,0,0, 1.0 };

	char* rgb = (char*)malloc(3 * width * height * sizeof(char));
	int x, y;
	for (x = 0; x<width; x++) {
		for (y = 0; y<height; y++) {
            //if (!(y < 583 && y > 581))
            //    continue;
			Ray r = getRay(x, y);
			
			Color c = traceRay_Plane(&r, &basePlane);
			
            int ipos = 3 * (width * y + x);

			rgb[ipos + 2] = (unsigned char)c.z;
			rgb[ipos + 1] = (unsigned char)c.y;
			rgb[ipos] = (unsigned char)c.x;
		}
	}
	write_bmp("test.bmp", width, height, rgb);
	 free(rgb);
     
     //system("pause");
	return 0;
}