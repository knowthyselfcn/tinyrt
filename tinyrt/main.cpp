#include "definition.h"
#include "cuboid.h"
#include "intersect.h"
//#include "sphere.h"
#include "shade.h"

#include "bmp.h"
#include "rtmath.h"

#include <math.h>
#include <float.h>

#include <iostream>

Point light = { 0, 5, 0 };		// 点光源的位置
                         // 世界坐标系



double distance =  1.5; //  眼睛与视窗的距离
const static int width = 640  ;  // 640 pixel    0.64m
const static int height = 480  ;

// view plane  的指定方式： eye space中 －z 垂直通过view plane
// view plane 的宽边与eye space y 轴（up）平行
// x,y 是像素坐标, 一个像素0.1cm，故view plane 宽64cm， 类似于25寸屏幕
Ray getRay(int x, int y ) {  

    Point eye = {0, 10, 20}; 

    Vector up = { 0, 1, 0 };  // 头顶的方向, 局部坐标系y，    // 世界坐标系 
    Vector lookAt = { 0, 0, 0 };  //眼睛向前的方向  

    //up = {0, 0, -1};   // 向地面俯视
    //lookAt = {0, -10, 0};

    Ray r;
    r.origin = eye;         // 设定眼睛的位置

    Vector tmpW = pointDifference(eye, lookAt);
    Vector w = normalize( &tmpW ) ;
    Vector tmpU = crossVector(&up, &w);
    Vector u = normalize(&tmpU);
    Vector tmpV = crossVector(&w, &u);
    Vector v = normalize(&tmpV);
    
    Vector rup = localToWorld(&up, &u, &v, &r.origin);

    Vector rw = {-w.x, -w.y, -w.z};

    Vector a = scalarVector(&rw, distance);
    
    double viewPlaneHeight = ((double)height) / 1000;
    Vector b = scalarVector(&v, viewPlaneHeight / 2 - ((double)y)/1000);
    
    double viewPlaneWidth = ((double)width) / 1000;
    Vector c = scalarVector(&u, - viewPlaneWidth / 2 + ((double)x) / 1000);

    r.direction = vectorAdd(a, vectorAdd(b,  c));   //

    //std::cout << vectorLength(r.direction) << "\t" << r.direction.x << "\t" << r.direction.y << "\t" << r.direction.z << std::endl;
    r.viewPlanePos = pointAdd(r.origin, r.direction);

    r.px = x; r.py = y; // for debug

    return r;
}



// 判断与哪个物体首次相交
Intersect getFirstIntersection(Ray* ray, Object *objs[], int num)
{
    Intersect intersection = {0,0,0, -1};
    List *list = NULL, *curr = NULL, *nearestIntersect = NULL;

    // 收集相交的物体
    int index = -1;
    for (int i = 0; i < num; i++)
    {
        Object* obj = objs[i];
        bool intersected = false;
        switch (obj->type)
        {
        case SPHERE:
            intersected = intersectSphere(ray, (Sphere*)obj->o, &intersection);
            break;
        case PLANE:
            intersected = intersectPlane(ray, (Plane*)obj->o, &intersection);
            break;
        case CUBOID:
            intersected = intersectCuboid(ray, (Cuboid*)obj->o, &intersection);
        default:
            break;
        }

        if (intersected){
            curr = (List*)malloc(sizeof(List));
            intersection.objectId = i;
            curr->intersect = intersection;
            curr->next = NULL;

            if (i == 1)
                int j = i;

            if (NULL == list)
                list = curr;
            else
                list->next = curr;
        }
        else {

        }
    }
    // 需要根据向量长度判断哪个最近
    double minLength = DBL_MAX;
    curr = list;


    if (ray->px == width / 2 && ray->py == height / 2)
        int pppp = width;

    while (curr) {
        Vector ez = pointDifference(curr->intersect.point, ray->origin);
        double vectorLen = vectorLength(ez);
        if (minLength > vectorLen) {
            minLength = vectorLen;
            nearestIntersect = curr;
            
            intersection = curr->intersect;
            //std::cout << intersection.point.z << std::endl;
        }
        curr = curr->next;
    }
    
    return intersection;
}





// 针对每一条视线，都需要判断首次与哪个物体相交： 判断交点到眼睛的距离即可
Color traceRay(Ray* ray, Object *objs[], int num)
{
    Color color = { 0, 0, 0 };  //默认黑色背景

    for (int i = 0; i < num; i++)
    {
        Intersect intersection = getFirstIntersection(ray, objs, num);
        if (-1 == intersection.objectId) {
            // no intersect
           // std::cout << "sss" << std::endl;
        }
        else {
            Object* obj = objs[intersection.objectId];
            // 开始着色
            switch (obj->type)    
            {
            case SPHERE:
                color = shade_Sphere(ray, (Sphere*)obj->o, &intersection, &light);
                break;
            case PLANE:
                color = shade_Plane(ray, (Plane*)obj->o, &intersection, &light);
                break;
            case CUBOID:
                color = shade_Cuboid(ray, (Cuboid*)obj->o, &intersection, &light);
            default:
                break;
            }
        }
    }

    return color;
}

// ground up P59
Color traceRay_Sphere(Ray* ray, Sphere* sphere)
{
    Color color = { 255, 0, 0 };
    // 求视线与球体的距离  设眼睛坐标e，圆心c， 视线与 ec 的夹角为 theta
    Vector ec = pointDifference(sphere->center, ray->origin);
    Vector rayDir = normalize(&ray->direction);
    double cos_theta = scalarProduct(&ec, &rayDir) / vectorLength(ec) / vectorLength(rayDir);
    double theta = acos(cos_theta);
    double d = vectorLength(ec) * sin(theta);   // 球心到直线距离

    Vector ce = pointDifference(ray->origin, sphere->center);
    Vector l = normalize(&rayDir);

    Vector temp = pointDifference(ray->origin, sphere->center);
    double a = scalarProduct(&rayDir, &rayDir);
    double b = 2 * scalarProduct(&temp, &rayDir);
    double c = scalarProduct(&temp, &temp) - sphere->radius * sphere->radius;
    double disc = b * b - 4 * a*c;
 
    
    //if (d > sphere->radius) {
    if (disc > 0) {
        //求相交点，有一点和两点，两点时后面的交点被忽略
        double e = sqrt(disc);
        double t = -(b + e) /( a * 2.0);
       // std::cout << disc << std::endl;
        if (t >= 0) {
            double tmin = t;
            Intersect intersection;
            Vector ttttttttt = vectorAdd(temp, scalarVector(&rayDir, t));
            Vector normal = scalarVector(&ttttttttt, sphere->radius);
            intersection.point = pointAdd(ray->origin, scalarVector(&rayDir, t));
            
            Vector rayToLigthVector = pointDifference(light, intersection.point);
            Ray rayToLight;
            rayToLight.origin = intersection.point;
            rayToLight.direction = rayToLigthVector;

            double intersectRayLen = vectorLength(rayToLigthVector);
            double length = intersectRayLen;
            double scale = 1 / (length * length * 0.025 + length * 0.001 + 1);  // + 0.03 * length

            bool isRayToLightSphereIntersecting = doesRaySphereIntersect(&rayToLight, sphere);
            if (isRayToLightSphereIntersecting) {
                color = {0};        // no light, dark
            }
            else {
                color.x *= scale;				// 模拟光源光衰减
                color.y *= scale;
                color.z *= scale;
            }
             
        }
        else {
            //std::cout << t << std::endl;
        }
    }
    else {
         color = {255, 255, 255};   // sky 
         //std::cout << "no sphere  intersection" << std::endl;
    }


    return color;
}


// 计算直线与平面的交点
/*
设眼睛位置E，视线向量v，平面经过点P，其法线n与E同侧， E投影到平面的点为E'，视线与平面交点Z， EP 与 EE' 夹角theta(fixed)， E 到平面距离h, EZ 与 -n 夹角 beta,
| Z - E | = l =  h /  cos(beta) = EP * cos(theta) / (dot(v,n) / length(v) / length(n) )
*/
Color traceRay_Plane(Ray* ray, Plane* plane) {
	Color color = { 0, 0, 0 };

    Vector reverseN = reverseVector(plane->normal);
    double cos_beta = scalarProduct(&ray->direction, &reverseN) / vectorLength(ray->direction) / vectorLength(reverseN);

    if (cos_beta < 0) {
        // no intersect  展示天空
        color = { 255, 255, 255 };
    }
    else {
        Vector ep = pointDifference(plane->p, ray->origin);
        double cos_theta = scalarProduct(&ep, &reverseN) / vectorLength(ep) / vectorLength(reverseN);
        double h = vectorLength(ep) * cos_theta;
        
        Vector normaledRay = normalize(&ray->direction);
        Vector ez = scalarVector(&normaledRay, (h / cos_beta));
       
        Point z = pointAdd(ray->origin, ez);

        Intersect intersect  ;
        intersect.point = z;

        Vector directionToLight = pointDifference(light, intersect.point);

        double length = vectorLength(directionToLight);

        double scale = 1 / (length * length * 0.005 + length * 0.01 + 1 );  // + 0.03 * length

        color = { 255, 0, 0 };      //intersect.plane->color;
        color.x *= scale;				// 模拟光源光衰减
        color.y *= scale;
        color.z *= scale;
    }

	return color;
}


int main(int argc, char* argv[]) {
	Plane basePlane = { 0, 1, 0,   0,0,0,  255, 0, 0 };    // xoz 平面
    Sphere sphere = {0, 0.5, 0,      0.5 };
    Cuboid cuboid = { 1, 1, 0, 2, 1, 1, 2, 1, -1,  1.414213562373 };

    Object obj1;
    obj1.type = PLANE;
    obj1.o = (void*)&basePlane;
    Object obj2;
    obj2.type = SPHERE;
    obj2.o = (void*)& sphere;

    Object* objs[] = { &obj1, &obj2 };  // 

	char* rgb = (char*)malloc(3 * width * height * sizeof(char));
	int x, y;
	for (x = 0; x<width; x++) {     // 列扫描
		for (y = 0; y<height; y++) {
			Ray r = getRay(x, y);
			
            //Color c = traceRay_Plane(&r, &basePlane);
            //Color c = traceRay_Sphere(&r, &sphere);

            int objSize = sizeof(objs) / sizeof(Object*);
            Color c = traceRay(&r, objs, objSize);

            int ipos = 3 * (width * y + x);

			rgb[ipos + 2] = (unsigned char)c.z;
			rgb[ipos + 1] = (unsigned char)c.y;
			rgb[ipos] = (unsigned char)c.x;
		}
	}
	write_bmp("test.bmp", width, height, rgb);
	free(rgb);
     
     //system("pause");
	return 0;
}