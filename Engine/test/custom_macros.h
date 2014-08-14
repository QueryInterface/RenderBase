#pragma once
#include "MathBasics.h"

#define EPSILON 0.000001
#define EPSILON_VEC vector3f_t(EPSILON, EPSILON, EPSILON)
#define COMPARE_FLOAT(val0, val1) (val0 > val1 - EPSILON && val0 < val1 + EPSILON)
//#define COMPARE_FLOAT_VEC(val0, val1) COMPARE_FLOAT((val0).x, (val1).x) && COMPARE_FLOAT((val0).y, (val1).y) && COMPARE_FLOAT((val0).z, (val1).z)
#define COMPARE_FLOAT_VEC(val0, val1) EXPECT_NEAR((val0).x, (val1).x, EPSILON); EXPECT_NEAR((val0).y, (val1).y, EPSILON); EXPECT_NEAR((val0).z, (val1).z, EPSILON)

static vector3f_t zero(0,0,0);
static vector3f_t one(1,1,1);
static vector3f_t x(1, 0, 0);
static vector3f_t y(0, 1, 0);
static vector3f_t z(0, 0, 1);