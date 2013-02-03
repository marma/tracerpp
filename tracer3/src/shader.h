#ifndef __SHADER_H__
#define __SHADER_H__

#include "vector.h"
#include "color.h"
#include "ray.h"
#include "intersection.h"
#include "serializable.h"

namespace tracer {

  class shade_return {
  public:
    shade_return(color _c, color _trans=opaque) : c(_c), trans(_trans) {}
    color c, trans;
  };
  
  class scene;
  
  class shade_context {
  public:
    shade_context(
		  const vector& _p,
		  const vector& _n,
		  const vector& _e,
		  scene *_scene,
		  const double& _time) : P(_p), N(_n), E(_e), world(_scene), time(_time) {
    }
    
    shade_context(
		  const vector& _E,
		  scene *_scene,
		  const intersection& i) : P(i.model_p), N(i.model_n), E(_E), world_P(i.world_p), world_N(i.world_n), world(_scene), time(i.time) {
    }
    
    vector P, N, E, world_P, world_N;
    scene *world;
    double time,s,t,u,v;
  };
  
  
  class shader : public serializable {
  public:
    shader(char *name) : serializable(name) {}
    virtual color ambient(const shade_context& context) {
      return tracer::black;
    }
    virtual color shade(const ray& ray, const shade_context& context) =0;
    virtual color transparency(const shade_context& context) {
      return opaque;
    }
    virtual shader* clone() =0;
  };
}

#endif
