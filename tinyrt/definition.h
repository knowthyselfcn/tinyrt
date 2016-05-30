#ifndef RT_DEFINITION_H
#define RT_DEFINITION_H

#define _USE_MATH_DEFINES

static const double epsilon = 0.000000000000001;

typedef unsigned char byte;

typedef struct {
	double  x;
	double 	y;
	double 	z;
} Vector;
typedef Vector Point;
typedef Vector Color;

typedef struct {
    byte r;
    byte g;
    byte b;
} RGBColor;

typedef struct {
    double  x;
    double 	y;
    double 	z;
    double  w;
} Vector4;

enum RayType
{
    EYE_RAY,
    REFL_RAY,
    REFR_RAY,
    LIGHT_RAY,
    _Ray_Type_Size
};

typedef struct {
	Point origin;
	Vector direction;
    Vector viewPlanePos;
    RayType type;
#ifdef DEBUG
    int px, py;   // for debug
#endif
} Ray;

typedef  Ray Line;  // 直线

 
typedef struct {
	Vector normal;
    Point p;
	Color color;
} Plane;		// 无限宽广的平面, 且只是与象平面平行

// 仔细选择p， v1, v2 需保证rect 法向对外
typedef struct {
	Point p;  //
	Vector v1;
	Vector v2;
} Rectangle;

typedef struct {
    Point p;
    Vector v1;
    Vector v2;
} Triangle;

typedef struct {
	Point center;
	double radius;
} Sphere;


typedef struct {
    Vector u;
    Vector v;
    Vector w;

    double height;  // view plane
    double width;

    Vector eye2Center;

    Point eye;
} EyeSpace;


typedef struct {
    Point point;
    int objectId;   // index from 0 in  Object[],  if -1, means no intersect
    int rectIdx;    // 为了box 的调试
} Intersect;

// 只需要指定底部的长宽，剩下的类似y轴，只指定长度即可，
// 底部面序号1，逆时针， 顶部为3，  前面5， 后面 6，  
typedef struct {
    Point p;   
    Vector wVector;   // widht  = x 
    Vector hVector;   // z
    double yLenth;      // y

    Rectangle rectangles[6];
} Cuboid;



typedef struct  {
    double m[3][3];
} Mat3;

typedef struct {
    double m[4][4];
} Mat4;

typedef struct {
    double m[4][1];
} Mat4_1;

enum TYPE {
    PLANE = 0,
    RECTANGLE ,
    TRIANGLE,

    SPHERE,
    CUBOID,
    CONE,
    FRUSTRUM
};

enum MATERIAL_TYPE {
    MATTE,
    PLASTIC,
    METAL,
    SILK,
    _MATERIAL_TYPE_SIZE
};

typedef struct {
    float	kd;
    Color 	cd;
} Lambertian;

typedef struct {
    Lambertian ambient_brdf;
    Lambertian diffuse_brdf;
} Matte;

typedef struct {
    MATERIAL_TYPE type;
    void *data;
} Material;

// 简单的示例程序中，这种简单的结构就没有存在的必要了，直接在上层代码中实现即可


// need to free the source manually
typedef struct {
    enum TYPE type;
    void *o;
    Material material;
} Object;



typedef struct _List{
    Intersect intersect ;
    struct _List* next ;
} List;


enum LIGHTTYPE
{
    POINT_LIGHT,
    DIRECTION_LIGHT,
    AMBIENT_LIGHT,
};


typedef struct {
    Vector pos;
    Vector color;
} PointLight;

typedef struct {
    Vector dir;
    Vector color;
    double ls;
} DirectionLight;

typedef struct {
    Color color;
    double ls;  // scale_radiance
} AmbientLight;

// 环境光遮挡功能实现有两种方式，
//1： 通过基于半球采样的相关材质来实现，需要指定各个对象的材质
//2： 可以使用下面的新的光源代替上面的 Env_light
typedef struct {
    Vector u;
    Vector v;
    Vector w;
    Color min_amount;
    //Sampler sampler_p;
} AmbientOccluderLight;

typedef struct {
    void *light;
    LIGHTTYPE type;
} Light;

typedef struct {
    Light data[64];  // 最多支持 64 个光源
    int size;       
} Lights;


typedef struct {
    Object objs[1024];
    int size;  // 最多能够放置的物体数量 1024

    Lights lights; // 光源集合




} World;

typedef struct {
    double x;
    double y;
} Point2d;


typedef struct {
    int num_samples; // the number of sample points in a pattern
    int num_sets;    // the number of sample sets(patterns) stored
    Point2d *samples; // 动态内存，需要注意管理
    int *shuffed_indicies;
    unsigned long count;
    int jump;

} Sampler;





#endif 