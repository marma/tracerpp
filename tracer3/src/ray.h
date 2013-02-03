#ifndef __RAY_H__
#define __RAY_H__

#include "vector.h"
#include "color.h"

namespace tracer {
  class ray {
  public:
    inline ray(const vector& _p, const vector& _v, double _t, const color& _c = color(0,0,0)) :  p(_p), v(_v), c(_c), t(_t) { }
    inline vector operator () (double _t) const {
      return p + _t*v;
    }
    
    vector p, v;
    color c;
    double t;
  };
}

#endif
