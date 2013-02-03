#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

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
#include "camera.h"
#include "imager.h"

#define PI 3.1415926535

namespace tracer {
  class raytracer : public serializable {
  public:
    raytracer();
    color filter(ray& r, double t0, double t1, scene& world);
    color raytrace(const ray& r, scene& world, int maxdepth = 32);
    color ambient(const vector& p, const vector& n, scene& world, double angle = PI, int nsamples = -1, double t = 0);
    virtual std::ostream& toStream(std::ostream& out) const =0;
    virtual std::istream& fromStream(std::istream& in) =0;
  protected:
    camera c;
    imager i;
    scene *s;
    int nAmbientSamples;
  };
}

#endif
