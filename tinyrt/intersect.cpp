#define _USE_MATH_DEFINES

#include "intersect.h"


int pointInRectangle_counter = 0;

// 如果相交，则 objectId 为 正数， 否则为-1
bool intersectSphere(Ray* ray, Sphere* sphere, Intersect* intersect)
{
    bool intersected = false;

    Vector rayDir = normalize(&ray->direction);
    Vector ce = pointDifference(&ray->origin, &sphere->center);
    
    double a = scalarProduct(&rayDir, &rayDir);
    double b = 2 * scalarProduct(&ce, &rayDir);
    double c = scalarProduct(&ce, &ce) - sphere->radius * sphere->radius;
    double disc = b * b - 4 * a*c;

    if (disc > 0) {
        //求相交点，有一点和两点，两点时后面的交点被忽略
        double e = sqrt(disc);
        double t = -(b + e) / (a * 2.0);
        if (t >= 0) {
            intersect->point = pointAdd(&ray->origin, &scalarVector(&rayDir, t));
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
// 150w 次相交 ， 1536000
bool intersectPlane(Ray* ray, Plane* plane, Intersect* intersect)
{
    bool intersected = false;

    //Vector reverseN = reverseVector(&plane->normal);
    Vector reverseN = scalarVector(&plane->normal, -1);
    double cos_beta = scalarProduct(&ray->direction, &reverseN) / vectorLength(&ray->direction) / vectorLength(&reverseN);

    if (cos_beta <= 0) {
        // no intersect  展示天空
        intersected = false;
    }
    else {
        Vector ep = pointDifference(&plane->p, &ray->origin);
        double cos_theta = scalarProduct(&ep, &reverseN) / vectorLength(&ep) / vectorLength(&reverseN);
        double h = vectorLength(&ep) * cos_theta;

        Vector normaledRay = normalize(&ray->direction);
        Vector ez = scalarVector(&normaledRay, (h / cos_beta));

        Point z = pointAdd(&ray->origin, &ez);

        intersect->point = z;
       
        intersected = true;
    }


    return intersected;
}


// between two vectors
inline bool betweenVectors(Vector* v1, Vector* v2, Vector* v)
{
    bool in = false;

    double t1 = scalarProduct(&crossVector(v1, v), &crossVector(v1, v2));
    double t2 = scalarProduct(&crossVector(v2, v), &crossVector(v2, v1));

    if (t1 >= 0 && t2 >= 0) {
        in = true;
    }

    return in;
}

// 1000w 次相交
bool pointInRectangle(Point* z, Rectangle* rect)
{
    bool in = false;

    Vector pz = pointDifference(z, &rect->p);

    bool between = betweenVectors(&rect->v1, &rect->v2, &pz);
    // 以v1为x，v2为y，构造坐标系， 判断 pz 是否在第一象限
    if ( between) {
        double lpz = vectorLength(&pz);
        Vector v3 = vectorAdd(&rect->v1, &rect->v2);  // 对角向量
        double cos_beta = scalarProduct(&v3, &rect->v1) / vectorLength(&v3) / vectorLength(&rect->v1);   // 下半部夹角
        double cos_theta = scalarProduct(&pz, &rect->v1) / vectorLength(&pz) / vectorLength(&rect->v1);
        double lpzp = 0;
        if (cos_theta >= cos_beta) {
            double lv1 = vectorLength(&rect->v1);  // length of v1
            lpzp = lv1 / cos_theta; // length of pz`   z` 为 pz直线与 v2` 的交点
        } 
        else  {  //上三角部分的判断  
            double lv2 = vectorLength(&rect->v2);  // length of v2
            double theta = acos(cos_theta);   // 与 v1 夹角
            lpzp = lv2 / sin(theta);
        }
        if (lpz <= lpzp) {
            in = true;
        }
    }

    return in;
}




// 640 * 480 * 5 * 6 光长方体就有 一千万次调用 耗费了4秒时间。    相交测试果然花费了很多时间
// pointInRectangle  612w 次，这就有点夸张了 耗时2.5s
bool intersectRect(Ray* ray, Rectangle* rect, Intersect* intersect)
{
   
    bool intersected = false;
    // 平行否
    Vector rectNormalDir = crossVector(&rect->v1, &rect->v2); //法向是唯一的，按逆时针方向
    double  cos_theta = scalarProduct(&ray->direction, &rectNormalDir)
            / vectorLength(&ray->direction) / vectorLength(&rectNormalDir);  // 钝角

    if (cos_theta < 0 )  {  // theta < M_PI_2
        Vector pe = pointDifference(&ray->origin,  &rect->p);  // eye 与 平面已知点p 连线
        // 投影线 与 已知固定点线 的夹角，求出 ee` （垂直投影线h）
        double cos_beta = scalarProduct(&pe, &rectNormalDir) / vectorLength(&pe) / vectorLength(&rectNormalDir);

        if (cos_beta > 0) { //beta < M_PI_2
            double h = vectorLength(&pe) * cos_beta;
            double theta = acos(cos_theta);
            double l = h / sin(theta - M_PI_2 );   // z 为直线穿过平面点， l 为 ez 长度
            Vector rayNormal = normalize(&ray->direction);
            Vector ez = scalarVector(&rayNormal, l);
            Point z = pointAdd(&ray->origin, &ez);
            bool in = pointInRectangle(&z, rect);
            if (in) {
                intersect->point = z;
                intersected = true;
            }
            else {

            }
        }
        //  else  方向不对，内侧相交，不考虑
        // TODO 需要考虑平行的状况 
    }
    //  else 与法相垂直， 什么都看不到  abs(theta - M_PI) < epsilon
    // > M_PI_2 内侧相交

    return intersected;
}



// 判断是否在v1，v2 夹角 cos_beta 内
// 参见 http://stackoverflow.com/questions/13640931/how-to-determine-if-a-vector-is-between-two-other-vectors  
bool pointInTriangle(Point* z, Triangle* triangle)
{
    bool in = false;

    Vector* v1 = &triangle->v1;
    Vector* v2 = &triangle->v2;

    Vector pz = pointDifference(z, &triangle->p);  

    double t1 = scalarProduct(&crossVector(v1, &pz), &crossVector(v1, v2));
    double t2 = scalarProduct(&crossVector(v2, &pz), &crossVector(v2, v1));

    // between two vectors
    if (t1 >= 0 && t2 >= 0) {
        Vector pz = pointDifference(z, &triangle->p);
        double lpz = vectorLength(&pz);

        double cos_theta = scalarProduct(&pz, &triangle->v1) / vectorLength(&pz) / vectorLength(&triangle->v1);
        double theta = acos(cos_theta);

        Vector rv1 = { -triangle->v1.x, -triangle->v1.y, -triangle->v1.z };
        Vector v3 = vectorAdd(&rv1, &triangle->v2);  // 以v1 终点为起点的 第三条边
        double cos_gamma = scalarProduct(&v3, &rv1) / vectorLength(&v3) / vectorLength(&rv1);
        double gamma = acos(cos_gamma);

        double lv1 = vectorLength(&triangle->v1);  // length of v1

        // length of pz`   z` 为 pz直线与 v3 的交点
        double h = lv1 * tan(theta) * tan(gamma) / (tan(theta) + tan(gamma));

        double lpzp = h / sin(theta);
        if (lpz <= lpzp) {
            in = true;
        }
 
    }

    return in;
}

bool intersectTriangle(Ray* ray, Triangle* triangle, Intersect* intersect)
{
    bool intersected = false;
    // 平行否
    Vector rectNormalDir = crossVector(&triangle->v1, &triangle->v2); //法向是唯一的，按逆时针方向

    double  cos_theta = scalarProduct(&ray->direction, &rectNormalDir)
        / vectorLength(&ray->direction) / vectorLength(&rectNormalDir);

    if (cos_theta < 0)  {  // theta < M_PI_2
        Vector ep = pointDifference(&ray->origin, &triangle->p);  // eye 与 平面已知点p 连线
        // 投影线 与 已知固定点线 的夹角，求出 ee` （垂直投影线h）
        double cos_beta = scalarProduct(&ep, &rectNormalDir) / vectorLength(&ep) / vectorLength(&rectNormalDir);
        if (cos_beta > 0) { //beta < M_PI_2
            double h = vectorLength(&ep) * cos_beta;
            double theta = acos(cos_theta);
            double l = h / sin(theta - M_PI_2);   // z 为直线穿过平面点， l 为 ez 长度
            Vector rayNormal = normalize(&ray->direction);
            Vector ez = scalarVector(&rayNormal, l);
            Point z = pointAdd(&ray->origin, &ez );
            bool in = pointInTriangle(&z, triangle);
            if (in) {
                intersect->point = z;
                intersected = true;
            }

        }
        // else 方向不对，内侧相交，不考虑
    }
    // else与法相垂直， 什么都看不到  abs(theta - M_PI) < epsilon
    // > M_PI_2 内侧相交
 
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