#define _USE_MATH_DEFINES

#include "intersect.h"


// ����ཻ���� objectId Ϊ ������ ����Ϊ-1
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
        //���ཻ�㣬��һ������㣬����ʱ����Ľ��㱻����
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






// ����ֱ����ƽ��Ľ���
/*
���۾�λ��E����������v��ƽ�澭����P���䷨��n��Eͬ�࣬ EͶӰ��ƽ��ĵ�ΪE'��������ƽ�潻��Z�� EP �� EE' �н�theta(fixed)�� E ��ƽ�����h, EZ �� -n �н� beta,
| Z - E | = l =  h /  cos(beta) = EP * cos(theta) / (dot(v,n) / length(v) / length(n) )
*/
bool intersectPlane(Ray* ray, Plane* plane, Intersect* intersect)
{
    //Intersect intersection = { 0, 0, 0, -1 };
    bool intersected = false;

    Vector reverseN = reverseVector(plane->normal);
    double cos_beta = scalarProduct(&ray->direction, &reverseN) / vectorLength(ray->direction) / vectorLength(reverseN);

    if (cos_beta <= 0) {
        // no intersect  չʾ���
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

    Vector v3 = vectorAdd(rect->v1, rect->v2);  // �Խ�����

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
            double lpzp = lv1 / cos_theta; // length of pz`   z` Ϊ pzֱ���� v2` �Ľ���
            if (lpz <= lpzp) {
                in = true;

                if (z->y < 0)
                    int jjjjj = 10;
            }
            else {
                // ���ⲿ
            }
        } 
        else if (cos_theta >0 && theta < M_PI_2) {  //�����ǲ��ֵ��ж�  
            double complementary_theta = M_PI_2 - theta;
            //double cos_complementary_theta = scalarProduct(&pz, &rect->v2) / vectorLength(pz) / vectorLength(rect->v2);
            //double lpzp = lv2 / cos_complementary_theta;
            double lpzp = lv2 / cos(complementary_theta);
            if (lpz <= lpzp) {
                in = true;
            }
            else {
                // ���ⲿ
            }
        }
        else {
            int jjjjjjjj = 99;
        }
    }
    else {
        // ��������һ�������ڣ�
    }

    return in;
}


bool intersectRect(Ray* ray, Rectangle* rect, Intersect* intersect)
{
    bool intersected = false;
    // ƽ�з�
    Vector rectNormalDir = crossVector(&rect->v1, &rect->v2); //������Ψһ�ģ�����ʱ�뷽��
    Vector rectNormal = normalize(&rectNormalDir);
    Vector rnormal = { -rectNormal.x, -rectNormal.y, -rectNormal.z };
    double  cos_theta = scalarProduct(&ray->direction, &rnormal)
        / vectorLength(ray->direction) / vectorLength(rnormal);
   // printf("%f\n", cos_theta);
    //double theta = acos(cos_theta);

    if (cos_theta > 0 )  {  // theta < M_PI_2
        Vector ep = pointDifference(rect->p, ray->origin);  // eye �� ƽ����֪��p ����
        // ͶӰ�� �� ��֪�̶����� �ļнǣ���� ee` ����ֱͶӰ��h��
        double cos_beta = scalarProduct(&ep, &rnormal) / vectorLength(ep) / vectorLength(rnormal);
        //double beta = acos(cos_beta);
        if (cos_beta > 0) { //beta < M_PI_2
            double h = vectorLength(ep) * cos_beta;
            double l = h / cos_theta;   // z Ϊֱ�ߴ���ƽ��㣬 l Ϊ ez ����
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
            // ���򲻶ԣ��ڲ��ཻ��������
            int i = 3;
        }
    }
    else {
        // �뷨�ഹֱ�� ʲô��������  abs(theta - M_PI) < epsilon
        // > M_PI_2 �ڲ��ཻ
    }


    return intersected;
}


// �����ڰ�Χ���ཻ����
bool intersectCuboid(Ray* ray, Cuboid* cuboid, Intersect* intersect)
{
    bool intersected = false;

    // ����������
    Rectangle *rectangles = cuboid->rectangles;


    Point center;

    // ����������
    //int rectIdx = -1;
    for (int i = 0; i < 6; i++)
    {
        Rectangle* rect = &rectangles[i];
        // �����ж�����ཻ���������ڲ�
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