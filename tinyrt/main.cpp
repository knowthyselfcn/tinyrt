#include "definition.h"
#include "intersect.h"
#include "shade.h"

#include "bmp.h"
#include "rtmath.h"

#include <math.h>
#include <float.h>

#include <iostream>

//Point light = { 0, 5, 0 };		// 点光源的位置
                         // 世界坐标系

Lights lights; // 光源集合



void initLights()
{
    // 正上方的点光源
    PointLight pointLight = { 0, 5, 0,  255, 0,0};
    add_PointLight(&lights, pointLight);

    //DirectionLight dLight = {0,-1,0,    0,128, 0}; // 半绿色
    //add_DirectiontLight(&lights, dLight);

    int i = 23;
}

bool destroyLights()
{
    return false;
}


double distance =  1.5; //  眼睛与视窗的距离
const static int width = 640  ;  // 640 pixel    0.64m
const static int height = 480  ;

// view plane  的指定方式： eye space中 －z 垂直通过view plane
// view plane 的宽边与eye space y 轴（up）平行
// x,y 是像素坐标, 一个像素0.1cm，故view plane 宽64cm， 类似于25寸屏幕
EyeSpace eyeSpace;

bool initEyeSpace()
{
    eyeSpace.eye = { 0, 5, 50 };
    Vector up = { 0, 1, 0 };  // 头顶的方向, 局部坐标系y，    // 世界坐标系 
    Vector lookAt = { 0, 0, 0 };  //眼睛向前的方向  
    //up = {0, 0, -1};   // 向地面俯视
    //lookAt = {0, -10, 0};

    Vector tmpW = pointDifference(eyeSpace.eye, lookAt);
    eyeSpace.w = normalize(&tmpW);
    Vector tmpU = crossVector(&up, &eyeSpace.w);
    eyeSpace.u = normalize(&tmpU);
    Vector tmpV = crossVector(&eyeSpace.w, &eyeSpace.u);
    eyeSpace.v = normalize(&tmpV);

    //Vector rup = localToWorld(&up, &u, &v, &r.origin); //

    Vector  rw = { -eyeSpace.w.x, -eyeSpace.w.y, -eyeSpace.w.z };
    eyeSpace.eye2Center = scalarVector(&rw, distance); //eye 到 view plane 中心的向量
    eyeSpace.height = ((double)height) / 1000;
    eyeSpace.width = ((double)width) / 1000;

    return true;
}

Ray getRay(int x, int y ) {  
 
    Ray r;
    r.origin = eyeSpace.eye;         // 设定眼睛的位置


    Vector b = scalarVector(&eyeSpace.v, eyeSpace.height / 2 - ((double)y)/1000);

    Vector c = scalarVector(&eyeSpace.u, -eyeSpace.width / 2 + ((double)x) / 1000);

    r.direction = vectorAdd(eyeSpace.eye2Center, vectorAdd(b,  c));   //

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
            break;
        case RECTANGLE:
            intersected = intersectRect(ray, (Rectangle*)obj->o, &intersection);
            break;
        case TRIANGLE:
            intersected = intersectTriangle(ray, (Triangle*)obj->o, &intersection);
            break;
        default:
            std::cout << "obj not checked intersection, bad logic" << std::endl;
            break;
        }

        // 标记
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

    // TODO release List mem
    curr = list;  
    while (curr) {
        List* next = curr->next;
        free((void*)curr);
        curr = next;
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
                color = shade_Sphere(ray, (Sphere*)obj->o, &intersection, &lights);
                break;
            case PLANE:
                color = shade_Plane(ray, (Plane*)obj->o, &intersection, &lights);
                break;
            case CUBOID:
                color = shade_Cuboid(ray, (Cuboid*)obj->o, &intersection, &lights);
                break;
            case  RECTANGLE:
                color = shade_Rectange(ray, (Rectangle*)obj->o, &intersection, &lights);
                break;
            case  TRIANGLE:
                color = shade_Triangle(ray, (Triangle*)obj->o, &intersection, &lights);
                break;
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
            
            Vector* light = (Vector*)(lights.data[0].light);
            

            Vector rayToLigthVector = pointDifference(*light, intersection.point);
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
        Vector* light = (Vector*)lights.data[0].light;
        Vector directionToLight = pointDifference(*light, intersect.point);

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
    initEyeSpace();
    initLights();

	Plane basePlane = { 0, 1, 0,   0,0,0,  255, 0, 0 };    // xoz 平面
    Sphere sphere = {0, 0.5, 0,      1 };
    
    Cuboid cuboid = { 1, 0, 0,  1, 0, -1,    1, 0, 1,  1.414213562373 };   // obj3
    Point p = cuboid.p;
    Vector tmpYDir = crossVector(&cuboid.hVector, &cuboid.wVector);
    Vector normaledY = normalize(&tmpYDir);
    Vector yVec = scalarVector(&normaledY, cuboid.yLenth);
    Vector tmpVec = vectorAdd(vectorAdd(cuboid.wVector, cuboid.hVector), yVec);   // 对角向量
    Point pp = pointAdd(p, tmpVec);
    Vector wVec = cuboid.wVector;
    Vector hVec = cuboid.hVector;
    // 取对角点， 两点各自对应三个面0,3,5,     1,2,4
    Vector rwVec = { -wVec.x, -wVec.y, -wVec.z };
    Vector rhVec = { -hVec.x, -hVec.y, -hVec.z };
    Vector ryVec = { -yVec.x, -yVec.y, -yVec.z };

    // 逆时针方向，即可判断
    cuboid.rectangles[0] = { p,  wVec,  hVec };  // 
    cuboid.rectangles[1] = { p,  yVec,  wVec, };
    cuboid.rectangles[2] = { pp, rhVec, rwVec, };
    cuboid.rectangles[3] = { pp, rwVec, ryVec, };
    cuboid.rectangles[4] = { pp, ryVec, rhVec, };
    cuboid.rectangles[5] = { p,  hVec,  yVec, };

    Rectangle rect = { -3, 0.5, 5,  1.5, 0, 0,      0, 1, -1, };
    Triangle triangle = { 4, 0.5, 0, 1.5, 0, 0, 0, 1, -1, };

    Object obj1 = { PLANE, (void*)&basePlane };
    Object obj2 = { SPHERE, (void*)& sphere };
    Object obj3 = { CUBOID, (void*)& cuboid };
    Object obj4 = { RECTANGLE, (void*)& rect };
    Object obj5 = { TRIANGLE, (void*)& triangle };

    Object* objs[] = { &obj1, &obj2, &obj3, &obj4, &obj5 };  //  

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