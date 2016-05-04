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






// 需要考虑可见面、eye 是否在光源同一侧, 此判断需要在shade计算过程中做
// 还原到简单的
/*
                |
    L.          |           .L`
                |
                |
                |
                |
 E.             |E`
                |
                |
                |
                |
                .P
                |
*/
Color shade_Cuboid(Ray* ray, Cuboid* cuboid, Intersect* intersect,   Vector* light)
{
    Color color = { 0, 0, 255 };
#ifdef DEBUG
    switch (intersect->rectIdx)
    {
    case 0:
        color = { 255, 0, 0 };
        break;
    case 1:
        color = { 0, 255, 0 };
        break;
    case 2:
        color = { 0, 0, 255 };  // 
        break;
    case 3:
        color = { 128, 128, 0 };  // 
        break;
    case 4:
        color = { 128, 0, 128 };
        break;
    case 5:
        color = { 128, 128, 128 };  //
        break;
    default:
        break;
    }
#endif


    Vector directionToLight = pointDifference(*light, intersect->point);

    double length = vectorLength(directionToLight);

    double scale = 1 / (length * length * 0.005 + length * 0.01 + 1);  // + 0.03 * length
    //printf("%f\n", scale);
    color.x *= scale;				// 模拟光源光衰减
    color.y *= scale;
    color.z *= scale;

    return color;
}


Color shade_Rectange(Ray* ray, Rectangle* cuboid, Intersect* intersect, Vector* light)
{
    Color color = { 0, 0, 255 };



    return color;
}
 