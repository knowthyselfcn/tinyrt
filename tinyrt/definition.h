#ifndef RT_DEFINITION_H
#define RT_DEFINITION_H


static const double epsilon = 0.000000000000001;



typedef struct {
	double  x;
	double 	y;
	double 	z;
} Vector;
typedef Vector Point;
typedef Vector Color;

typedef struct {
    double  x;
    double 	y;
    double 	z;
    double  w;
} Vector4;

typedef struct {
	Point origin;
	Vector direction;
} Ray;

typedef  Ray Line;  // 直线

 
typedef struct {
	Vector normal;
    Point p;
	Color color;
} Plane;		// 无限宽广的平面, 且只是与象平面平行

typedef struct {
	Point p;  //
	Vector v1;
	Vector v2;
} Rectangle;

typedef struct {
	Point center;
	double radius;
} Sphere;

typedef struct {
	Point point;
	double lambda;
	Plane* plane;
} Intersection;







typedef struct  {
    double m[3][3];
} Mat3;

typedef struct {
    double m[4][4];
} Mat4;

typedef struct {
    double m[4][1];
} Mat4_1;


#endif 