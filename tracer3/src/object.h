#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "vector.h"
#include "color.h"
#include "ray.h"
#include "intersection.h"
#include "shape.h"
#include "shader.h"
#include "volumeshader.h"
#include "vector_function.h"


namespace tracer {
  class object {
  public:
    // constructors and destructors
    object(shape *_shp = NULL, shader *_shd = NULL, volumeshader *_vsh = NULL, vector_function *_tr = NULL) : _shape(_shp), _shader(_shd), _volumeshader(_vsh), _translation(_tr) {
    }
    
    // methods
    void intersect(const ray& r, intersection_vector& ivect) {
      intersection_collection ic;
      
      // 1) transform ray into object coordinate system
      vector x(0,0,0);
      
      if (_translation != NULL) {
	x += (*_translation)(r.t);
      }
      
      ray newR = ray(r.p - x, r.v, r.t);
      
      // 2) intersect shape with ray
      _shape->intersect(newR, ic);
      
      /* 3) transform intersections back into world coordinate system,
	 add pointer to object and 
	 add intersection to 'ivect' */
      for (intersection_collection::iterator iter = ic.begin(); iter != ic.end(); iter++) {
	iter->world_p = iter->model_p + x;
	iter->world_n = iter->model_n;
	iter->t = (r.p - iter->world_p).length();
	iter->obj = this;
	
	ivect.push_back(*iter);
      }
    }
    
    int inside(const vector& p, double t) {
      // 1) transform vector into object coordinates
      // 2) check if inside
      return (_translation != NULL)? _shape->inside(p - (*_translation)(t),t):_shape->inside(p,t);
    }
    
    int operator == (const object& obj) {
      return _shape == obj._shape && _shader == obj._shader && _volumeshader == obj._volumeshader && _translation == obj._translation;
    }
    
    // friends
    friend std::ostream& operator << (std::ostream& out, const object& o) {
      if (o._shape != NULL) {
	out << " shape " << *o._shape;
      }
      
      if (o._shader != NULL) {
	out << " shader " << *o._shader;
      }
      
      if (o._volumeshader != NULL) {
	//out << " volume_shader " << *o._volumeshader;
      }
      
      return out;
    }
    
    // data members	    
    shape *_shape;
    shader *_shader;
    volumeshader *_volumeshader;
    vector_function *_translation;
  };
  
  typedef std::list<object> object_collection;
}

#endif
