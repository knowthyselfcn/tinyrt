#ifndef RT_DEFINITION_H
#define RT_DEFINITION_H


static const double epsilon = 0.000000000000001;

typedef struct {
	double  x;
	double 	y;
	double 	z;
} Vector;
typedef Vector Point;
typedef Vector Color;

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
    __Ray_Type_Size
};

typedef struct {
	Point origin;
	Vector direction;
    Vector viewPlanePos;
#ifdef DEBUG
    int px, py;   // for debug
    RayType type;
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

// need to free the source manually
typedef struct {
    enum TYPE type;
    void *o;
} Object;



typedef struct _List{
    Intersect intersect ;
    struct _List* next ;
} List;


enum LIGHTTYPE
{
    POINT_LIGHT,
    DIRECTION_LIGHT,
    ENV_LIGHT,
};


typedef struct {
    Vector pos;
    Vector color;
} PointLight;

typedef struct {
    Vector dir;
    Vector color;
} DirectionLight;

typedef struct {
    Color color;
} Env_light;

typedef struct {
    void *light;
    LIGHTTYPE type;
} Light;

typedef struct {
    Light data[64];  // 最多支持64 个光源
    int size;       
} Lights;




#endif 