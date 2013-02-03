#ifndef __INTERSECTION_H__
#define __INTERSECTION_H__

#include <list>
#include <vector>
#include "vector.h"

namespace tracer {
  class object; 

  class intersection {
  public:
    enum { ENTER, EXIT, UNKNOWN };
    inline intersection(const vector& _model_p, const vector& _model_n, double _t, double _time, int _type = UNKNOWN, \
			double _u=0, double _v=0) : model_p(_model_p), model_n(_model_n), t(_t), time(_time), u(_u), v(_v), type(_type) {}
    inline int operator < (const intersection& i) const {
      return t < i.t;
    }
    
    object *obj;
    vector model_p,model_n, world_p, world_n;
    double t,time,u,v,bary_r,bary_s,bary_t;
    int type;
  };
  
  typedef std::list<intersection> intersection_collection;
  typedef std::vector<intersection> intersection_vector;
}

#endif
