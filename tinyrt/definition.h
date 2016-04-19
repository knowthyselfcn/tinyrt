#ifndef RT_DEFINITION_H
#define RT_DEFINITION_H

#define epsilon 0.000000001



typedef struct {
	double  x;
	double 	y;
	double 	z;
} Vector;
typedef Vector Point;
typedef Vector Color;

typedef struct {
	Point origin;
	Vector direction;
} Ray;

typedef struct {
	Vector normal;
	double distance;
	Color color;
} Plane;

typedef struct {
	Point point;
	double lambda;
	Plane* plane;
} Intersection;

#endif 