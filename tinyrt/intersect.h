#ifndef TINYRT_INTERSECT_H
#define TINYRT_INTERSECT_H
#include "definition.h"
#include "rtmath.h"

#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <assert.h>


bool intersectSphere(Ray* ray, Sphere* sphere, Intersect* intersect);

bool intersectPlane(Ray* ray, Plane* plane, Intersect* intersect);


bool intersectCuboid(Ray* ray, Cuboid* cuboid, Intersect* intersect);

bool intersectRect(Ray* ray, Rectangle* rect, Intersect* intersect);


bool intersectTriangle(Ray* ray, Triangle* triangle, Intersect* intersect);


Intersect getFirstIntersection(Ray* ray, Object *objs[], int num);








#endif