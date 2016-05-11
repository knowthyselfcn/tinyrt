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



double distance = 1.5; //  眼睛与视窗的距离
const static int width = 640;  // 640 pixel    0.64m
const static int height = 480;


void initLights(World *world)
{
    // 正上方的点光源
    PointLight pointLight = { 0, 5, 0,  255, 0,0};
    add_PointLight(&world->lights, pointLight);

    DirectionLight dLight = { -1, 0, 0,    30, 30, 30}; // 半绿色
    add_DirectiontLight(&world->lights, dLight);

    int i = 23;
}

bool destroyLights()
{
    return false;
}


// view plane  的指定方式： eye space中 －z 垂直通过view plane
// view plane 的宽边与eye space y 轴（up）平行
// x,y 是像素坐标, 一个像素0.1cm，故view plane 宽64cm， 类似于25寸屏幕
EyeSpace eyeSpace;

bool initEyeSpace()
{
    eyeSpace.eye = { 0, 10, 30 };
    Vector up = { 0, 1, -1 };  // 头顶的方向, 局部坐标系y，    // 世界坐标系 
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

    Vector bc = vectorAdd(&b,  &c);
    r.direction = vectorAdd(&eyeSpace.eye2Center, &bc);   //
    r.viewPlanePos = pointAdd(&r.origin, &r.direction);
    r.type = EYE_RAY;
#ifdef DEBUG
    r.px = x; r.py = y; // for debug
#endif
    return r;
}







// 针对每一条视线，都需要判断首次与哪个物体相交： 判断交点到眼睛的距离即可
Color traceRay(Ray* ray, World *world)
{
    int num = world->size;

    Color color = { 0, 0, 0 };  //默认黑色背景

    for (int i = 0; i < num; i++)
    {
        Intersect intersection = getFirstIntersection(ray, world);
        if (-1 == intersection.objectId) {
            // no intersect
           // std::cout << "sss" << std::endl;
        }
        else {
            Object* obj = &world->objs[intersection.objectId];
            // 开始着色
            switch (obj->type)    
            {
            case SPHERE:
                color = shade_Sphere(ray, (Sphere*)obj->o, &intersection, &world->lights);
                break;
            case PLANE:
                color = shade_Plane(ray, (Plane*)obj->o, &intersection, &world->lights, world);
                break;
            case CUBOID:
                color = shade_Cuboid(ray, (Cuboid*)obj->o, &intersection, &world->lights);
                break;
            case  RECTANGLE:
                color = shade_Rectange(ray, (Rectangle*)obj->o, &intersection, &world->lights);
                break;
            case  TRIANGLE:
                color = shade_Triangle(ray, (Triangle*)obj->o, &intersection, &world->lights);
                break;
            default:
                break;
            }
        }
    }

    return color;
}
 

void test() 
{
    Vector n = { 1, 0, 0 };
    Vector v = { -1, 1, 0 };
    Vector reflVec = getReflect(&n, &v);


    std::cout << "d";
}

 
#include <chrono>

// 传入的对象都会被copy一份
bool addObjs2World(World* world, Object* o)
{
    if (world->size < 1024) {
        Object *obj = &world->objs[world->size];
        switch (o->type)
        {
        case PLANE:
            obj->type = PLANE;
            obj->o = malloc(sizeof(Plane) );
            memcpy(obj->o, o->o, sizeof(Plane));
            break;
        case SPHERE:
            obj->type = SPHERE;
            obj->o = malloc(sizeof(Sphere));
            memcpy(obj->o, o->o, sizeof(Sphere));
            break;
        case RECTANGLE:
            obj->type = RECTANGLE;
            obj->o = malloc(sizeof(Rectangle) );
            memcpy(obj->o, o->o, sizeof(Rectangle));
            break;
        case CUBOID:
            obj->type = CUBOID;
            obj->o = malloc(sizeof(Cuboid));
            memcpy(obj->o, o->o, sizeof(Cuboid));
            break;
        case TRIANGLE:
            obj->type = TRIANGLE;
            obj->o = malloc(sizeof(Triangle));
            memcpy(obj->o, o->o, sizeof(Triangle));
            break;

        default:
            break;
        }
        world->size++;
    }
    else {

    }
    return false;
}

bool buildWorld(World* world)
{
    Plane basePlane = { 0, 1, 0, 0, 0, 0, 255, 0, 0 };    // xoz 平面

    Sphere sphere = { 0, 0.5, 0, 1 };

    Cuboid cuboid = { -3, 0, 0, 1, 0, -1, 1, 0, 1, 1.414213562373 };   // obj3
    Point p = cuboid.p;
    Vector tmpYDir = crossVector(&cuboid.hVector, &cuboid.wVector);
    Vector normaledY = normalize(&tmpYDir);
    Vector yVec = scalarVector(&normaledY, cuboid.yLenth);
    Vector whVec = vectorAdd(&cuboid.wVector, &cuboid.hVector);
    Vector tmpVec = vectorAdd(&whVec, &yVec);   // 对角向量
    Point pp = pointAdd(&p, &tmpVec);
    Vector wVec = cuboid.wVector;
    Vector hVec = cuboid.hVector;
    // 取对角点， 两点各自对应三个面0,3,5,     1,2,4
    Vector rwVec = { -wVec.x, -wVec.y, -wVec.z };
    Vector rhVec = { -hVec.x, -hVec.y, -hVec.z };
    Vector ryVec = { -yVec.x, -yVec.y, -yVec.z };

    // 逆时针方向，即可判断
    cuboid.rectangles[0] = { p, wVec, hVec };  // 
    cuboid.rectangles[1] = { p, yVec, wVec, };
    cuboid.rectangles[2] = { pp, rhVec, rwVec, };
    cuboid.rectangles[3] = { pp, rwVec, ryVec, };
    cuboid.rectangles[4] = { pp, ryVec, rhVec, };
    cuboid.rectangles[5] = { p, hVec, yVec, };

    Rectangle rect = { -3, 0.5, 1, 2, 0, 0, 0, 2, -2, };
        rect = { 1, 1, 1, 2, 0, 0, 0, 1, -1, };  // for debug Rectangle

    Triangle triangle = { 3, 1.5, 1, 2, 0, 0, 0, 1, -2, };
   

    Object objPlane = { PLANE, (void*)& basePlane };
    addObjs2World(world, &objPlane);
    Object objSphere = { SPHERE, (void*)& sphere };
    addObjs2World(world, &objSphere);
    Object objCuboid = { CUBOID, (void*)& cuboid };
    addObjs2World(world, &objCuboid);
    Object objRect = { RECTANGLE, (void*)& rect };
    addObjs2World(world, &objRect);
    Object objTriangle = { TRIANGLE, (void*)& triangle };
    addObjs2World(world, &objTriangle);

    return true;
}

    
extern   int pointInRectangle_counter ;
int main(int argc, char* argv[]) {
    //test();
    auto start = std::chrono::high_resolution_clock::now();

    initEyeSpace();

    World world;
    world.size = 0;
    buildWorld(&world);

    initLights(&world);

	
    Object *objs = world.objs;

	char* rgb = (char*)malloc(3 * width * height * sizeof(char));
 
    //Ray r;
    // x, y 一定要放到下面来定义，否则就错了      //Color c;    private(c)  private(r) 
#define USE_OMP
#ifdef USE_OMP
#pragma omp parallel for schedule(dynamic, 8) 
#endif
	for (int x = 0; x<width; x++) {     // 列扫描
		for (int y = 0; y<height; y++) {
            Ray r = getRay(x, y);

            //int objSize = sizeof(objs) / sizeof(Object*);
            int objSize = world.size;
            Color c = traceRay(&r, &world);

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