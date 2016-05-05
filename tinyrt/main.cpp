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
    eyeSpace.eye = { 0, 5, 30 };
    Vector up = { 0, 1, 0 };  // 头顶的方向, 局部坐标系y，    // 世界坐标系 
    Vector lookAt = { 0, 0, 0 };  //眼睛向前的方向  
    //up = {0, 0, -1};   // 向地面俯视
    //lookAt = {0, -10, 0};

    Vector tmpW = pointDifference(&eyeSpace.eye, &lookAt);
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

    Vector b = scalarVector(&eyeSpace.v,  eyeSpace.height / 2 - ((double)y)/1000);
    Vector c = scalarVector(&eyeSpace.u, -eyeSpace.width / 2  + ((double)x)/1000);

    r.direction = vectorAdd(&eyeSpace.eye2Center, &vectorAdd(&b,  &c));   //
    r.viewPlanePos = pointAdd(&r.origin, &r.direction);

    //r.px = x; r.py = y; // for debug

    return r;
}



// 判断与哪个物体首次相交
Intersect getFirstIntersection(Ray* ray, Object *objs[], int num)
{
    Intersect intersection = {0,0,0, -1};
    List *list = NULL, *curr = NULL, *nearestIntersect = NULL;

    // 收集相交的物体
    //int index = -1;
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

    while (curr) {
        Vector ez = pointDifference(&curr->intersect.point, &ray->origin);
        double vectorLen = vectorLength(&ez);
        if (minLength > vectorLen) {
            minLength = vectorLen;
            nearestIntersect = curr;
            
            intersection = curr->intersect;
        }
        curr = curr->next;
    }

    // release List mem
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
 

 
#include <chrono>


    

extern   int pointInRectangle_counter ;
int main(int argc, char* argv[]) {
    auto start = std::chrono::high_resolution_clock::now();

    initEyeSpace();
    initLights();

	Plane basePlane = { 0, 1, 0,   0,0,0,  255, 0, 0 };    // xoz 平面
    Sphere sphere = {0, 0.5, 0,      1 };
    
    Cuboid cuboid = { 1, 0, 0,  1, 0, -1,    1, 0, 1,  1.414213562373 };   // obj3
    Point p = cuboid.p;
    Vector tmpYDir = crossVector(&cuboid.hVector, &cuboid.wVector);
    Vector normaledY = normalize(&tmpYDir);
    Vector yVec = scalarVector(&normaledY, cuboid.yLenth);
    Vector tmpVec = vectorAdd(&vectorAdd(&cuboid.wVector, &cuboid.hVector), &yVec);   // 对角向量
    Point pp = pointAdd(&p, &tmpVec);
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
 
    Ray r;
    // x, y 一定要放到下面来定义，否则就错了
#pragma omp parallel for schedule(dynamic, 4)  private (r)
	for (int x = 0; x<width; x++) {     // 列扫描
		for (int y = 0; y<height; y++) {
			r = getRay(x, y);

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
     
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    std::cout << pointInRectangle_counter << std::endl;

    system("pause");
	return 0;
}