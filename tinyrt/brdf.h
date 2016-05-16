#ifndef TINYRT_BRDF_H
#define TINYRT_BRDF_H

#include "definition.h"

Color matte_ambient_brdf(Object *obj, double ka, double rho);

Color matte_diffuse_brdf();



Color plastic_ambient_brdf();

Color plastic_diffuse_brdf();

Color plastic_specular_brdf();

 
/*
散射、ambient occlusion 这两部分的确有一些难度，看了这些材料，还是无法实现出来。

*/


#endif
