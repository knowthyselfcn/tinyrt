#include "definition.h"

#include "bmp.h"


#include <math.h>
#include <float.h>



Ray getRay(int x, int y) {
	Ray r;
	r.origin.x = 0;
	r.origin.y = 0;
	r.origin.z = 0;
	r.direction.x = -320 + x + 0.5;
	r.direction.y = 240 - y + 0.5;
	r.direction.z = -120;
	return r;
}

Plane* createBox() {
	Plane* box = (Plane*)malloc(5 * sizeof(Plane));
	int size = 5;
	Plane p0 = { -1, 0, 0, size, 255, 0, 100 };//links
	box[0] = p0;
	Plane p1 = { 1, 0, 0, size, 0, 255, 0 };//rechts
	box[1] = p1;
	Plane p2 = { 0, 0, -1, size, 255, 255, 255 };//hinten
	box[2] = p2;
	Plane p3 = { 0, 1, 0, size, 255, 255, 255 };//oben
	box[3] = p3;
	Plane p4 = { 0, -1, 0, size, 255, 255, 255 };//unten
	box[4] = p4;
	return box;
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

Intersection rayHitsPlane(Ray* r, Plane* p)  {
	double gamma = scalarProduct(&(r->direction), &(p->normal));
	double side = p->distance - scalarProduct(&(p->normal), &(r->origin));
	Intersection intersect = { 0, 0, 0, 0, p };
	if (gamma * side > epsilon) {
		double lambda = side / gamma;
		intersect.point.x = r->origin.x + r->direction.x * lambda;
		intersect.point.y = r->origin.y + r->direction.y * lambda;
		intersect.point.z = r->origin.z + r->direction.z * lambda;
		intersect.lambda = lambda;
	}
	else {
	}

	return intersect;
}

Intersection getFirstIntersection(Ray* ray, Plane* box) {
	Intersection nearestIntersect; double minDouble = DBL_MAX;
	int planeCounter;
	for (planeCounter = 0; planeCounter < 5; planeCounter++) {
		Intersection intersect = rayHitsPlane(ray, &box[planeCounter]);
		if (intersect.lambda > epsilon && intersect.lambda < minDouble) {
			nearestIntersect = intersect;
			minDouble = intersect.lambda;
		}
	}
	return nearestIntersect;
}

Color traceRay(Ray* ray, Plane* box) {
	Color color = { 0, 0, 0 };
	Point light = { 0.5, 2, -3.5 };
	Intersection intersect = getFirstIntersection(ray, box);
	if (intersect.lambda > epsilon) {
		Vector directionToLight = pointDifference(light, intersect.point);
		Ray rayToLight = { intersect.point, directionToLight };
		Intersection tempIntersect = getFirstIntersection(&rayToLight, box);
		if (tempIntersect.lambda >= 1 || tempIntersect.lambda == 0) {
			double length = vectorLength(directionToLight);
			double scale = 5 / (length * length);
			color = intersect.plane->color;
			color.x *= scale;
			color.y *= scale;
			color.z *= scale;
		}
	}
	return color;
}

void  normalizeColor(Color* color) {
	double factor = 255 / fmax(color->x, fmax(color->y, color->z));
	if (factor < 1) {
		color->x = color->x*factor;
		color->y = color->y*factor;
		color->z = color->z*factor;
	}
}



const static int width = 640 * 2;
const static int height = 480 * 2;

int main(int argc, char* argv[]) {
	Plane* box = createBox();
	char* rgb = (char*)malloc(3 * width * height * sizeof(char));
	int x, y;
	for (x = 0; x<width; x++) {
		for (y = 0; y<height; y++) {
			Ray r = getRay(x, y);
			Color c = traceRay(&r, box);
			normalizeColor(&c);
			int ipos = 3 * (width * y + x);
			//double red = c.z;
			//double green = c.y;
			//double blue = c.x;
			rgb[ipos + 2] = (unsigned char)c.z;
			rgb[ipos + 1] = (unsigned char)c.y;
			rgb[ipos] = (unsigned char)c.x;
		}
	}
	write_bmp("test.bmp", width, height, rgb);
	free(box); free(rgb);
	return 0;
}