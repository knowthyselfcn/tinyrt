#include "shade.h"


Color shade_Sphere(Ray* ray, Sphere* sphere, Intersect* intersection, Vector* light)
{
    Color color = { 255, 0, 0 };

    Vector rayToLigthVector = pointDifference(*light, intersection->point);
    Ray rayToLight;
    rayToLight.origin = intersection->point;
    rayToLight.direction = rayToLigthVector;

    double intersectRayLen = vectorLength(rayToLigthVector);
    double length = intersectRayLen;
    double scale = 1 / (length * length * 0.025 + length * 0.001 + 1);  // + 0.03 * length

    bool isRayToLightSphereIntersecting = doesRaySphereIntersect(&rayToLight, sphere);
    if (isRayToLightSphereIntersecting) {
        color = { 0 };        // no light, dark
    }
    else {
        color.x *= scale;				// 模拟光源光衰减
        color.y *= scale;
        color.z *= scale;
    }

    return color;
}





Color shade_Plane(Ray* ray, Plane* plane, Intersect* intersect, Vector* light)
{
    Color color = { 255, 0, 0 };

    Vector directionToLight = pointDifference(*light, intersect->point);

    double length = vectorLength(directionToLight);

    double scale = 1 / (length * length * 0.005 + length * 0.01 + 1);  // + 0.03 * length

    color = { 255, 0, 0 };      //intersect.plane->color;
    color.x *= scale;				// 模拟光源光衰减
    color.y *= scale;
    color.z *= scale;


    return color;
}







Color shade_Cuboid(Ray* ray, Cuboid* cuboid, Intersect* intersect,   Vector* light)
{
    Color color = { 255, 0, 0 };



    return color;
}
 