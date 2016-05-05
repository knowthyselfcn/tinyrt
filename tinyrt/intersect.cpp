#define _USE_MATH_DEFINES

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

//
bool pointInRectangle(Point* z, Rectangle* rect)
{
    bool in = false;

    Vector v3 = vectorAdd(rect->v1, rect->v2);  // 对角向量

    Vector pz = pointDifference(*z, rect->p);
    double lpz = vectorLength(pz);

    double cos_beta = scalarProduct(&v3, &rect->v1) / vectorLength(v3) / vectorLength(rect->v1);
    double beta = acos(cos_beta);   //

    double lv1 = vectorLength(rect->v1);  // length of v1
    double lv2 = vectorLength(rect->v2);  // length of v2

    double cos_theta = scalarProduct(&pz, &rect->v1) / vectorLength(pz) / vectorLength(rect->v1);
    double cos_tt = scalarProduct(&pz, &rect->v2) / vectorLength(pz) / vectorLength(rect->v2);
    double theta = acos(cos_theta);
    if (cos_theta > 0 && 0 < theta && theta < M_PI_2 &&  cos_tt > 0) { // &&
        if (theta <= beta) {
            double lpzp = lv1 / cos_theta; // length of pz`   z` 为 pz直线与 v2` 的交点
            if (lpz <= lpzp) {
                in = true;

                if (z->y < 0)
                    int jjjjj = 10;  // FIXME
            }
            else {
                // 在外部
            }
        } 
        else if (cos_theta >0 && theta < M_PI_2) {  //上三角部分的判断  
            double complementary_theta = M_PI_2 - theta;
            //double cos_complementary_theta = scalarProduct(&pz, &rect->v2) / vectorLength(pz) / vectorLength(rect->v2);
            //double lpzp = lv2 / cos_complementary_theta;
            double lpzp = lv2 / cos(complementary_theta);
            if (lpz <= lpzp) {
                in = true;
            }
            else {
                // 在外部
            }
        }
        else {
            int jjjjjjjj = 99;   // FIXME
        }
    }
    else {
        // 根本不在一个象限内，
    }

    return in;
}


bool intersectRect(Ray* ray, Rectangle* rect, Intersect* intersect)
{
    bool intersected = false;
    // 平行否
    Vector rectNormalDir = crossVector(&rect->v1, &rect->v2); //法向是唯一的，按逆时针方向
    Vector rectNormal = normalize(&rectNormalDir);
    Vector rnormal = { -rectNormal.x, -rectNormal.y, -rectNormal.z };
    double  cos_theta = scalarProduct(&ray->direction, &rnormal)
        / vectorLength(ray->direction) / vectorLength(rnormal);
   // printf("%f\n", cos_theta);
    //double theta = acos(cos_theta);

    if (cos_theta > 0 )  {  // theta < M_PI_2
        Vector ep = pointDifference(rect->p, ray->origin);  // eye 与 平面已知点p 连线
        // 投影线 与 已知固定点线 的夹角，求出 ee` （垂直投影线h）
        double cos_beta = scalarProduct(&ep, &rnormal) / vectorLength(ep) / vectorLength(rnormal);
        //double beta = acos(cos_beta);
        if (cos_beta > 0) { //beta < M_PI_2
            double h = vectorLength(ep) * cos_beta;
            double l = h / cos_theta;   // z 为直线穿过平面点， l 为 ez 长度
            Vector rayNormal = normalize(&ray->direction);
            Vector ez = scalarVector(&rayNormal, l);
            Point z = pointAdd(ray->origin, ez);
            //printf("%f\t%f\n", h, l);
            bool in = pointInRectangle(&z, rect);
            if (in) {
                intersect->point = z;
                intersected = true;
            }
            else {

            }
        }
        else {
            // 方向不对，内侧相交，不考虑
            int i = 3;
        }
    }
    else {
        // 与法相垂直， 什么都看不到  abs(theta - M_PI) < epsilon
        // > M_PI_2 内侧相交
    }


    return intersected;
}
/*
Vector rv1 = { -triangle->v1.x, -triangle->v1.y, -triangle->v1.z};
Vector v3 = vectorAdd(rv1, triangle->v2);  // 以v1 终点为起点的 第三条边
double cos_gamma = scalarProduct(&v3, &triangle->v1) / vectorLength(v3) / vectorLength(triangle->v1);
double gamma = acos(cos_gamma);

Vector pz = pointDifference(*z, triangle->p);
double lpz = vectorLength(pz);

double cos_beta = scalarProduct(&triangle->v2, &triangle->v1) / vectorLength(triangle->v2) / vectorLength(triangle->v1);
double beta = acos(cos_beta);   //

double lv1 = vectorLength(triangle->v1);  // length of v1
double lv2 = vectorLength(triangle->v2);  // length of v2

// theta pz 与 v1 夹角，   tt  pz 与 v2 夹角
double cos_theta = scalarProduct(&pz, &triangle->v1) / vectorLength(pz) / vectorLength(triangle->v1);
double cos_tt = scalarProduct(&pz, &triangle->v2) / vectorLength(pz) / vectorLength(triangle->v2);
double theta = acos(cos_theta);
if (cos_theta > 0 && cos_tt > 0 && 0 < theta && theta < beta && acos(cos_tt) < beta  &&  gamma < M_PI_2) { //

}
else {
    // 根本不在一个象限内，
}
*/


// 判断是否在v1，v2 夹角 cos_beta 内
// 参见 http://stackoverflow.com/questions/13640931/how-to-determine-if-a-vector-is-between-two-other-vectors  
bool pointInTriangle(Point* z, Triangle* triangle)
{
    bool in = false;

    Vector v1 = triangle->v1;
    Vector v2 = triangle->v2;

    Vector v3 = pointDifference(*z, triangle->p);

    double t1 = scalarProduct(&crossVector(&v1, &v3), &crossVector(&v1, &v2));
    double t2 = scalarProduct(&crossVector(&v2, &v3), &crossVector(&v2, &v1));

    // between two vectors
    if (t1 >= 0 && t2 >= 0) {
        double cos_beta = scalarProduct(&triangle->v2, &triangle->v1) / vectorLength(triangle->v2) / vectorLength(triangle->v1);
        double beta = acos(cos_beta);   //

        Vector pz = pointDifference(*z, triangle->p);
        double lpz = vectorLength(pz);

        double cos_theta = scalarProduct(&pz, &triangle->v1) / vectorLength(pz) / vectorLength(triangle->v1);
        double theta = acos(cos_theta);

        Vector rv1 = { -triangle->v1.x, -triangle->v1.y, -triangle->v1.z };
        Vector v3 = vectorAdd(rv1, triangle->v2);  // 以v1 终点为起点的 第三条边
        double cos_gamma = scalarProduct(&v3, &rv1) / vectorLength(v3) / vectorLength(rv1);
        double gamma = acos(cos_gamma);

        double lv1 = vectorLength(triangle->v1);  // length of v1
        double lv2 = vectorLength(triangle->v2);  // length of v2

        // length of pz`   z` 为 pz直线与 v3 的交点
        double h = lv1 * tan(theta) * tan(gamma) / (tan(theta) + tan(gamma));
        if (theta > M_PI_2 || gamma > M_PI_2)
            int i = 9;
        //printf("%f\n", h);
        double lpzp = h / sin(theta);
        if (lpz <= lpzp) {
            in = true;
        }
        else {
            // 在外部
        }
    }
    else {

    }




    return in;
}

bool intersectTriangle(Ray* ray, Triangle* triangle, Intersect* intersect)
{
    bool intersected = false;
    // 平行否
    Vector rectNormalDir = crossVector(&triangle->v1, &triangle->v2); //法向是唯一的，按逆时针方向
    Vector rectNormal = normalize(&rectNormalDir);
    Vector rnormal = { -rectNormal.x, -rectNormal.y, -rectNormal.z };
    double  cos_theta = scalarProduct(&ray->direction, &rnormal)
        / vectorLength(ray->direction) / vectorLength(rnormal);
    // printf("%f\n", cos_theta);
    //double theta = acos(cos_theta);

    if (cos_theta > 0)  {  // theta < M_PI_2
        Vector ep = pointDifference(triangle->p, ray->origin);  // eye 与 平面已知点p 连线
        // 投影线 与 已知固定点线 的夹角，求出 ee` （垂直投影线h）
        double cos_beta = scalarProduct(&ep, &rnormal) / vectorLength(ep) / vectorLength(rnormal);
        //double beta = acos(cos_beta);
        if (cos_beta > 0) { //beta < M_PI_2
            double h = vectorLength(ep) * cos_beta;
            double l = h / cos_theta;   // z 为直线穿过平面点， l 为 ez 长度
            Vector rayNormal = normalize(&ray->direction);
            Vector ez = scalarVector(&rayNormal, l);
            Point z = pointAdd(ray->origin, ez);
            //printf("%f\t%f\n", h, l);
            bool in = pointInTriangle(&z, triangle);
            if (in) {
                intersect->point = z;
                intersected = true;
            }
            else {

            }
        }
        else {
            // 方向不对，内侧相交，不考虑
            int i = 3;
        }
    }
    else {
        // 与法相垂直， 什么都看不到  abs(theta - M_PI) < epsilon
        // > M_PI_2 内侧相交
    }


    return intersected;
}



// 类似于包围盒相交测试
bool intersectCuboid(Ray* ray, Cuboid* cuboid, Intersect* intersect)
{
    bool intersected = false;

    // 构造六个面
    Rectangle *rectangles = cuboid->rectangles;

    // 遍历六个面
    //int rectIdx = -1;
    for (int i = 0; i < 6; i++)
    {
        Rectangle* rect = &rectangles[i];
        // 必须判断外侧相交，而不是内侧
        intersected = intersectRect(ray, rect, intersect);
        if (intersected) {
            intersect->rectIdx = i;
            break;
        }
        else {

        }
    }

    return intersected;
}