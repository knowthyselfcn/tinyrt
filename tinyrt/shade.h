#ifndef TINYRT_SHADE_H
#define TINYRT_SHADE_H

#include "definition.h"
#include "rtmath.h"
#include "math.h"

bool addLight(Lights* lights, Light light);

// 内存管理在这里做
bool  add_PointLight(Lights* lights, PointLight light);
bool  remove_PointLight(Lights* lights, PointLight light);

bool  add_DirectiontLight(Lights* lights, DirectionLight light);
bool  add_AmbientLight(Lights* lights, AmbientLight light);

Color shade_Sphere(Ray* ray, Object* sphere, Intersect* intersection, Lights* lights);

Color shade_Plane(Ray* ray, Plane* plane, Intersect* intersect, Lights* lights, World *world);


Color shade_Cuboid(Ray* ray, Cuboid* cuboid, Intersect* intersect, Lights* lights);

Color shade_Rectange(Ray* ray, Rectangle* cuboid, Intersect* intersect, Lights* lights);

Color shade_Triangle(Ray* ray, Triangle* triangle, Intersect* intersect, Lights* lights);









#endif