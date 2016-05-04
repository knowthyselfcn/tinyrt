#ifndef TINYRT_SHADE_H
#define TINYRT_SHADE_H

#include "definition.h"
#include "rtmath.h"
#include "math.h"



Color shade_Sphere(Ray* ray, Sphere* sphere, Intersect* intersection, Vector* light);

Color shade_Plane(Ray* ray, Plane* plane, Intersect* intersect, Vector* light);


Color shade_Cuboid(Ray* ray, Cuboid* cuboid, Intersect* intersect,  Vector* light);

Color shade_Rectange(Ray* ray, Rectangle* cuboid, Intersect* intersect, Vector* light);

#endif