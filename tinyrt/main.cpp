#include "definition.h"

#include "bmp.h"


#include <math.h>
#include <float.h>

Point light = { 0, 5, 0 };		// 点光源的位置
const static int width = 640 * 2;
const static int height = 480 * 2;

Ray getRay(int x, int y) {
	Ray r;
	r.origin.x = 0;         // 设定眼睛的位置
	r.origin.y = 15;
	r.origin.z = 60;
	r.direction.x = -width / 2 + x + 0.5;	// left to right
	r.direction.y = height / 2 - y + 0.5;	// top to down
	r.direction.z = -400;
	return r;
}


double vectorLength(Vector v) {
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vector pointDifference(Point p1, Point p2) {
	Vector v = { p1.x - p2.x, p1.y - p2.y, p1.z - p2.z };
	return v;
}

double scalarProduct(Vector* v1, Vector* v2) {
	return v1->x*v2->x + v1->y*v2->y + v1->z*v2->z;
}


void  normalizeColor(Color* color) {
	double factor = 255 / fmax(color->x, fmax(color->y, color->z));
	if (factor < 1) {
		color->x = color->x*factor;
		color->y = color->y*factor;
		color->z = color->z*factor;
	}
}


Color traceRay_Plane(Ray* ray, Plane* plane) {
	Color color = { 0, 0, 0 };

	double gamma = scalarProduct(&(ray->direction), &(plane->normal));
	double side = plane->distance - scalarProduct(&(plane->normal), &(ray->origin));		// 有时候Ray 是
	Intersection intersect = { 0, 0, 0, 0, plane };
	if (gamma * side > epsilon) {
		double lambda = side / gamma;
		intersect.point.x = ray->origin.x + ray->direction.x * lambda;   // 模拟光源光衰减
		intersect.point.y = ray->origin.y + ray->direction.y * lambda;
		intersect.point.z = ray->origin.z + ray->direction.z * lambda;
		intersect.lambda = lambda;
	}
	else {
		 
	}


	Vector directionToLight = pointDifference(light, intersect.point);
	Ray lightRay = { intersect.point, directionToLight };

	if (intersect.lambda > epsilon) {
		double length = vectorLength(directionToLight);
		double scale = 175 / (length * length * 0.95 + 10 * length);
		color = intersect.plane->color;
		color.x *= scale;				// 模拟光源光衰减
		color.y *= scale;
		color.z *= scale;
	}
	else {
		//color.x = 215;	// 假设的天空颜色
		//color.y = 145;
	}

	return color;
}



int main(int argc, char* argv[]) {
	Plane basePlane = { 0, -1, 0, 5, 255, 0, 0 };

	char* rgb = (char*)malloc(3 * width * height * sizeof(char));
	int x, y;
	for (x = 0; x<width; x++) {
		for (y = 0; y<height; y++) {
			Ray r = getRay(x, y);
			
			Color c = traceRay_Plane(&r, &basePlane);

			normalizeColor(&c);
			int ipos = 3 * (width * y + x);

			rgb[ipos + 2] = (unsigned char)c.z;
			rgb[ipos + 1] = (unsigned char)c.y;
			rgb[ipos] = (unsigned char)c.x;
		}
	}
	write_bmp("test.bmp", width, height, rgb);
	 free(rgb);
	return 0;
}