#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "vector.h"
#include "ray.h"
#include "intersection.h"
#include "serializable.h"

namespace tracer {
  class shape : public serializable {
  public:
    shape(char *name) : serializable(name) {}
    virtual void intersect(const ray& r, intersection_collection& icoll) =0;
    virtual int inside(const vector& x, double t) =0;
    virtual shape *clone() =0;
  };
}

#endif
