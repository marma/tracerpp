#ifndef __LIGHTSOURCE_H__
#define __LIGHTSOURCE_H__

#include <list>
#include "vector.h"
#include "color.h"
#include "serializable.h"
#include "ray.h"

namespace tracer {
  class lightsource : public serializable {
  public:
    lightsource(char *name) : serializable(name) { }
    virtual ray shade(const vector& p, double t) =0;
    virtual int inside(const vector& p, double t) =0;
    virtual lightsource *clone() =0;
  };

  typedef std::list<lightsource*> lightsource_collection;
}

#endif
