#include "shade.h"
#include <stdlib.h>


bool addLight(Lights* lights, Light light)
{
    bool ret = false;
    if (lights->size <= 64){
        lights->data[lights->size] = light;
        lights->size++;

        ret = true;
    }
    else {

    }

    return ret;
}


bool  add_PointLight(Lights* lights, PointLight light)
{
    bool ret = false;

    Light pointLight;
    pointLight.light = (void*)malloc(sizeof(PointLight));
    pointLight.type = POINT_LIGHT;
    *(PointLight *)pointLight.light = { light };

    lights->data[lights->size] = pointLight;
    lights->size++;

    ret = addLight(lights, pointLight);
    if (false == ret) {
        free(pointLight.light);
    }
 

    return ret;
}

bool  remove_PointLight(Lights* lights, PointLight light)
{

    return false;
}


bool  add_DirectiontLight(Lights* lights, DirectionLight light)
{
    bool ret = false;
 
    Light dirLight;
    dirLight.light = (void*)malloc(sizeof(DirectionLight));
    dirLight.type = DIRECTION_LIGHT;
    *(DirectionLight *)dirLight.light = { light };

    ret = addLight(lights, dirLight);
    if (false == ret)
    {
        free(dirLight.light);
    }
 
    return ret;
}




Color shade_Sphere(Ray* ray, Sphere* sphere, Intersect* intersection, Lights* lights)
{
    Color color = { 255, 0, 0 };

    Vector* light = (Vector*)lights->data[0].light;

    Vector rayToLigthVector = pointDifference(light, &intersection->point);
    Ray rayToLight;
    rayToLight.origin = intersection->point;
    rayToLight.direction = rayToLigthVector;

    double intersectRayLen = vectorLength(&rayToLigthVector);
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





Color shade_Plane(Ray* ray, Plane* plane, Intersect* intersect, Lights* lights)
{
    Color color = { 255, 0, 0 };    // 最终计算出来的颜色
    Vector* light = (Vector*)lights->data[0].light;

    for (int i = 0; i < lights->size; i++) {
        Vector directionToLight = pointDifference(light, &intersect->point);
        double length = vectorLength(&directionToLight);
        double scale = 1 / (length * length * 0.005 + length * 0.01 + 1);  // + 0.03 * length

        color.x *= scale;				// 模拟光源光衰减
        color.y *= scale;
        color.z *= scale;
    }

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
Color shade_Cuboid(Ray* ray, Cuboid* cuboid, Intersect* intersect, Lights* lights)
{
    Color color = { 0, 0, 255 };
//#ifndef DEBUG
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
//#endif

    Vector* light = (Vector*)lights->data[0].light;
    Vector directionToLight = pointDifference(light, &intersect->point);

    double length = vectorLength(&directionToLight);

    double scale = 1 / (length * length * 0.005 + length * 0.01 + 1);  // + 0.03 * length
    //printf("%f\n", scale);
    color.x *= scale;				// 模拟光源光衰减
    color.y *= scale;
    color.z *= scale;

    return color;
}


Color shade_Rectange(Ray* ray, Rectangle* cuboid, Intersect* intersect, Lights* lights)
{
    Color color = { 0, 0, 200 };
    Vector* light = (Vector*)lights->data[0].light;
    Vector directionToLight = pointDifference(light, &intersect->point);

    double length = vectorLength(&directionToLight);

    double scale = 1 / (length * length * 0.005 + length * 0.01 + 1);  // + 0.03 * length
    //printf("%f\n", scale);
    color.x *= scale;				// 模拟光源光衰减
    color.y *= scale;
    color.z *= scale;


    return color;
}


Color shade_Triangle(Ray* ray, Triangle* triangle, Intersect* intersect, Lights* lights)
{
    Color color = { 0, 0, 200 };
    Vector* light = (Vector*)lights->data[0].light;
    Vector directionToLight = pointDifference(light, &intersect->point);

    double length = vectorLength(&directionToLight);

    double scale = 1 / (length * length * 0.005 + length * 0.01 + 1);  // + 0.03 * length
    //printf("%f\n", scale);
    color.x *= scale;				// 模拟光源光衰减
    color.y *= scale;
    color.z *= scale;


    return color;
}
 