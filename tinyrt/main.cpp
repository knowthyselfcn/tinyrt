#include "definition.h"

#include "bmp.h"
#include "rtmath.h"

#include <math.h>
#include <float.h>

#include <iostream>

Point light = { 0, 10, 0 };		// 点光源的位置
Point center = { 0, 0, 0 };     // 坐标系原点
Point eye = {0, 20, 320};                          // 世界坐标系
Vector lookAtDir = {0, 0, -8};  //眼睛向前的方向   // 世界坐标系
Vector up = {0, 1, 0};  // 头顶的方向, 局部坐标系，// 世界坐标系

double distance = 480; //  眼睛与视窗的距离
const static int width = 640 * 2;
const static int height = 480 * 2;



bool isLineParallel(Ray* r1, Ray* r2)
{
    return false;
}



// view plane  的指定方式： eye space中 －z 垂直通过view plane
// view plane 的宽边与eye space y 轴（up）平行
Ray getRay(int w, int h) {
	Ray r;
    r.origin = eye;         // 设定眼睛的位置

    Vector viewRight2Left = reverseVector(  crossVector(&lookAtDir, &up) );
    
    Vector rup = localToWorld(&up, &lookAtDir, &up, &eye);
    
    //testMultiplyMat3();
    
    //std::cout << rup.x << "\t" << rup.y << "\t" << rup.z << std::endl;
    

    r.direction = vectorAdd(scalarVector(&lookAtDir, distance),
        vectorAdd(scalarVector(&up, height / 2 - h), scalarVector(&viewRight2Left, -width / 2 + w)));
    
	//r.direction.x =  + 0.5;	// left to right
	//r.direction.y = height / 2 - h + 0.5;	// top to down
	//r.direction.z = -960;
	
    
    //std::cout << r.direction.x << "\t" << r.direction.y << "\t" << r.direction.z << std::endl;
    
    return r;
}



Color traceRay(Ray* ray, Plane* plane, Sphere* sphere)
{
    Color color = { 0, 0, 0 };
    // 针对每一条视线，都需要判断首次与哪个物体相交： 判断交点到眼睛的距离即可

    return color;
}

// 计算直线与平面的交点
/*
设眼睛位置E，视线向量v，平面经过点P，其法线n与E同侧， E投影到平面的点为E'，视线与平面交点Z， EP 与 EE' 夹角theta(fixed)， E 到平面距离h, EZ 与 -n 夹角 beta,
| Z - E | = l =  h /  cos(beta) = EP * cos(theta) / (dot(v,n) / length(v) / length(n) )
*/
Color traceRay_Plane(Ray* ray, Plane* plane) {
	Color color = { 0, 0, 0 };
    
    //std::cout << ray->direction.x << "\t" << ray->direction.y << "\t" << ray->direction.z << std::endl;

    Vector reverseN = reverseVector(plane->normal);
    double cos_beta = scalarProduct(&ray->direction, &reverseN) / vectorLength(ray->direction) / vectorLength(reverseN);

    //std::cout << cos_beta << std::endl;
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

        Intersection intersect = { z, 0, plane };

        Vector directionToLight = pointDifference(light, intersect.point);
        //Ray lightRay = { intersect.point, directionToLight };

        double length = vectorLength(directionToLight);
        

        double scale = 1 / (length * length *0.0001 + 0.003 * length +  1);
        
        //std::cout << ez.x << "\t" << length << "\t" << cos_beta << std::endl;

        color = intersect.plane->color;
        color.x *= scale;				// 模拟光源光衰减
        color.y *= scale;
        color.z *= scale;
    }

	return color;
}


int main(int argc, char* argv[]) {
	Plane basePlane = { 0, 1, 0,   0,0,0,  255, 0, 0 };    // xoz 平面

    Sphere sphere = {0,0,0,     1.0 };

	char* rgb = (char*)malloc(3 * width * height * sizeof(char));
	int x, y;
	for (x = 0; x<width; x++) {     // 列扫描
		for (y = 0; y<height; y++) {
            //if (!(y < 583 && y > 581))
            //    continue;
			Ray r = getRay(x, y);
			
			Color c = traceRay_Plane(&r, &basePlane);
			
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