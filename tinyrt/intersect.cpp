#include "intersect.h"


// 如果相交，则 objectId 为 正数， 否则为-1
bool intersectSphere(Ray* ray, Sphere* sphere, Intersect* intersect)
{
    bool intersected = false;

    Vector rayDir = normalize(&ray->direction);
    Vector ce = pointDifference(ray->origin, sphere->center);
    Vector l = normalize(&rayDir);

    Vector temp = pointDifference(ray->origin, sphere->center);
    double a = scalarProduct(&rayDir, &rayDir);
    double b = 2 * scalarProduct(&temp, &rayDir);
    double c = scalarProduct(&temp, &temp) - sphere->radius * sphere->radius;
    double disc = b * b - 4 * a*c;

    if (disc > 0) {
        //求相交点，有一点和两点，两点时后面的交点被忽略
        double e = sqrt(disc);
        double t = -(b + e) / (a * 2.0);
        if (t >= 0) {
            //double tmin = t;
            //Vector ttttt = vectorAdd(temp, scalarVector(&rayDir, t));
            //Vector normal = scalarVector(&, sphere->radius);
            intersect->point = pointAdd(ray->origin, scalarVector(&rayDir, t));
        }
        intersected = true;
    }
    else {
        intersected = false;
    }


    return intersected;
}






// 计算直线与平面的交点
/*
设眼睛位置E，视线向量v，平面经过点P，其法线n与E同侧， E投影到平面的点为E'，视线与平面交点Z， EP 与 EE' 夹角theta(fixed)， E 到平面距离h, EZ 与 -n 夹角 beta,
| Z - E | = l =  h /  cos(beta) = EP * cos(theta) / (dot(v,n) / length(v) / length(n) )
*/
bool intersectPlane(Ray* ray, Plane* plane, Intersect* intersect)
{
    //Intersect intersection = { 0, 0, 0, -1 };
    bool intersected = false;

    Vector reverseN = reverseVector(plane->normal);
    double cos_beta = scalarProduct(&ray->direction, &reverseN) / vectorLength(ray->direction) / vectorLength(reverseN);

    if (cos_beta <= 0) {
        // no intersect  展示天空
        intersected = false;
    }
    else {
        Vector ep = pointDifference(plane->p, ray->origin);
        double tttt = scalarProduct(&ep, &reverseN);
        double cos_theta = scalarProduct(&ep, &reverseN) / vectorLength(ep) / vectorLength(reverseN);
        double h = vectorLength(ep) * cos_theta;

        Vector normaledRay = normalize(&ray->direction);
        Vector ez = scalarVector(&normaledRay, (h / cos_beta));

        Point z = pointAdd(ray->origin, ez);

        intersect->point = z;

        intersected = true;
    }


    return intersected;
}


bool intersectRect(Ray* ray, Rectangle* rect, Intersect* intersect)
{

    return false;
}


// 类似于包围盒相交测试
bool intersectCuboid(Ray* ray, Cuboid* cuboid, Intersect* intersect)
{
    // 构造六个面
    Rectangle *rectangles = cuboid->rects;
    // 取对角点， 两点各自对应三个面0,3,5,     1,2,4
    Point p = cuboid->p;
    Vector tmpYDir = crossVector(&cuboid->wVector, &cuboid->hVector);
    Vector normaledY = normalize(&tmpYDir);
    Vector yVec = scalarVector(&normaledY, cuboid->yLenth);
    Vector tmpVec = vectorAdd(vectorAdd(cuboid->wVector, cuboid->hVector), yVec);   // 对角向量
    Point pp = pointAdd(p, tmpVec);

    Vector wVec = cuboid->wVector;
    Vector hVec = cuboid->hVector;

    rectangles[0] = { p, wVec, hVec };
    rectangles[1] = { pp, hVec, yVec };
    rectangles[2] = { pp, wVec, hVec };
    rectangles[3] = { p, hVec, yVec };
    rectangles[4] = { pp, wVec, yVec };
    rectangles[5] = { p, wVec, yVec };

    Point center;

    // 遍历六个面
    int rectIdx = -1;
    for (int i = 0; i < 6; i++)
    {
        Rectangle* rect = &rectangles[i];
        // 必须判断外侧相交，而不是内侧
        bool intersected = intersectRect(ray, rect, intersect);
        if (intersected) {

        }
        else {

        }
    }




    return false;
}