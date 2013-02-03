#ifndef __RAYTRACE_H__
#define __RAYTRACE_H__

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include "vector.h"
#include "color.h"
#include "serializable.h"
#include "ray.h"
#include "scene.h"
#include "math.h"

#define PI 3.1415926535

namespace tracer {
  color filter(ray& r, double t0, double t1, scene& world);
  color raytrace(const ray& r, scene& world, int maxdepth = 32);
  color preview(const ray& r, scene& world);
  color ambient(const vector& p, const vector& n, scene& world, double angle = PI, int nsamples = 8, double t = 0);
}

#endif
