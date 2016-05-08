#include "shade.h"
#include "intersect.h"

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







Color shade_Sphere_PointLight(Ray* ray, Sphere* sphere, Intersect* intersection, PointLight* light)
{
    Color color = { 0 };
    
    Color materialColor = light->color;
    
    Vector lightPos = light->pos;
    
    Vector rayToLigthVector = pointDifference(&lightPos, &intersection->point);
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
        color.x = materialColor.x * scale;				// 模拟光源光衰减
        color.y = materialColor.y * scale;
        color.z = materialColor.z * scale;
    }
 
    return color;
}

Color shade_Sphere_DirLight(Ray* ray, Sphere* sphere, Intersect* intersection, DirectionLight* light)
{
    Color color = { 0 };
    
    
    
    
    return color;
}


Color shade_Sphere(Ray* ray, Sphere* sphere, Intersect* intersection, Lights* lights)
{
    Color retColor = { 0 };  // result color
    
    
    for(int i=0; i < lights->size; i++) {
        Light light = lights->data[i];
        Color color;
        switch (light.type) {
            case POINT_LIGHT:
                color = shade_Sphere_PointLight( ray, sphere, intersection, (PointLight*)light.light);
                break;
            case DIRECTION_LIGHT:
                color = shade_Sphere_DirLight(ray, sphere, intersection, (DirectionLight*)light.light );
                
            default:
                break;
        }
        
        retColor.x += color.x;
        retColor.y += color.y;
        retColor.z += color.z;
    }
    
    return retColor;
}









Color shade_Plane_DirLight(Ray* ray, Plane* plane, Intersect* intersect, DirectionLight* light, Object *objs[], int num)
{
    Color retColor;
    
    Vector lightDir = light->dir;
    Color lightColor = light->color;
    
    
    Vector directionToLight = scalarVector(&lightDir, -1); //
    
    bool canReachLight = true;
    // TODO 还需要判断此 intersect 是否能够到达light, 若不能，则不shade
    Ray reflRay;
    reflRay.origin = intersect->point;
    reflRay.direction = directionToLight;    // 全向量
    reflRay.type = REFL_RAY;

    
    Intersect reflIntersect = getFirstIntersection(&reflRay, objs, num);
    if (-1 != reflIntersect.objectId)
    {
        Vector t = pointDifference(&reflIntersect.point, &intersect->point);
        double l = vectorLength(&t);
        double d = vectorLength(&directionToLight);
        if (l < epsilon)
            int i = 9;  // 自己
        else if (l < d) {
            canReachLight = false; // FIXME
        }
    }
    
    if (canReachLight){
        retColor = lightColor;
    }
    else {
        retColor.x *= 0;	 //  没有light 照到，所以是黑的
        retColor.y *= 0;
        retColor.z *= 0;
    }
    
    
    
    
    
    return retColor;
}



Color shade_Plane_EnvLight(Ray* ray, Plane* plane, Intersect* intersect, Env_light* light, Object *objs[], int num)
{
    Color color;
    
    
    
    
    
    return color;
    
}


Color shade_Plane_PointLight(Ray* ray, Plane* plane, Intersect* intersect, PointLight* light, Object *objs[], int num)
{

    
    Vector lightPos = light->pos;
    Color color = light->color;
    
    Vector directionToLight = pointDifference(&lightPos, &intersect->point);
    double length = vectorLength(&directionToLight);
    bool canReachLight = true;
    // TODO 还需要判断此 intersect 是否能够到达light, 若不能，则不shade
    Ray reflRay;
    reflRay.origin = intersect->point;
    reflRay.direction = directionToLight;    // 全向量
    reflRay.type = REFL_RAY;
    
#ifdef DEBUG
    double ll = 0.01;
    if (abs(intersect->point.x) < ll &&  abs(intersect->point.y) < ll   && abs(intersect->point.z) < ll) {
        int iiiii = 1;
        //canReachLight = false;
    }
#endif
    
    Intersect reflIntersect = getFirstIntersection(&reflRay, objs, num);
    if (-1 != reflIntersect.objectId)
    {
        Vector t = pointDifference(&reflIntersect.point, &intersect->point);
        double l = vectorLength(&t);
        double d = vectorLength(&directionToLight);
        if (l < epsilon)
            int i = 9;  // 自己
        else if (l < d) {
            canReachLight = false; // FIXME
        }
    }
    
    if (canReachLight){
        double scale = 1 / (length * length * 0.005 + length * 0.01 + 1);  // + 0.03 * length
        assert(scale < 1);
        color.x *= scale;				// 模拟光源光衰减
        color.y *= scale;
        color.z *= scale;
    }
    else {
        color.x *= 0;	 //  没有light 照到，所以是黑的
        color.y *= 0;
        color.z *= 0;
    }

    
    
    return color;
    
}


Color shade_Plane(Ray* ray, Plane* plane, Intersect* intersect, Lights* lights, Object *objs[], int num)
{
    Color retColor = { 0, 0, 0 };    // 最终计算出来的颜色
   

    for (int i = 0; i < lights->size; i++) {
        Light light = lights->data[i];
        Color color = { 0 };
        switch (light.type) {
            case POINT_LIGHT:
                color = shade_Plane_PointLight(ray, plane, intersect, (PointLight*)light.light, objs, num);
                break;
            case ENV_LIGHT:
                color = shade_Plane_EnvLight(ray, plane, intersect, (Env_light*)light.light, objs, num);
                break;
            case DIRECTION_LIGHT:
                color = shade_Plane_DirLight(ray, plane, intersect, (DirectionLight*)light.light, objs, num);
                break;
            default:
                break;
        }
        
        retColor.x += color.x;
        retColor.y += color.y;
        retColor.z += color.z;
        
        
        if (retColor.x > 256 || retColor.y > 256 || retColor.z > 256) {
//            printf("%f \t\t %f \t\t %f \n", retColor.x, retColor.y, retColor.z);
        }
        
      }

    return retColor;
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
 