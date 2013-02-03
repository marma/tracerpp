#include "raytrace.h"

namespace tracer {
  color filter(ray& r, double t0, double t1, scene& world) {
    intersection_vector ivect;
    object_collection inside_coll;
    
    // find intersections and check if we're inside objects
    for (object_collection::iterator obj_iter = world.objects.begin(); obj_iter != world.objects.end(); obj_iter++) {
      obj_iter->intersect(r, ivect);
      if (obj_iter->inside(r.p, r.t) && obj_iter->_volumeshader != NULL) inside_coll.insert(inside_coll.begin(), *obj_iter);
    }
    
    std::sort(ivect.begin(), ivect.end());
    
    color trans = tracer::white;
    vector p = r.p;
    double tp = 0;
    
    // iterate over all intersections
    intersection_vector::iterator ivect_iter;
    for (ivect_iter = ivect.begin(); ivect_iter != ivect.end(); ivect_iter++) {
      //shade_context shtx(ivect_iter->p, ivect_iter->n, r.v, &world, r.t);
      shade_context shtx(r.v, &world, *ivect_iter);

      // calculate volume filter
#define NSTEPS 10
      if (inside_coll.size() != 0) {	
	double len = ivect_iter->t - tp;
	color voltrans = black;
	
	for (object_collection::iterator obj_iter = inside_coll.begin(); obj_iter != inside_coll.end(); obj_iter++) {
	  voltrans += obj_iter->_volumeshader->filter(p, ivect_iter->model_p, r.t, NSTEPS);
	}
	
	//std::cout << (weight*voltrans).exp() << std::endl;
	
	trans *= (len*voltrans).exp();
      }
      
      if (ivect_iter->t >= t0 && ivect_iter->t <= t1-0.001) {
	trans *= ivect_iter->obj->_shader->transparency(shtx);
      }
      
      if (ivect_iter->obj->_volumeshader != NULL) {
	if (ivect_iter->type == intersection::ENTER) {
	  inside_coll.insert(inside_coll.begin(), *ivect_iter->obj);
	} else {
	  inside_coll.remove(*(ivect_iter->obj));		
	}
      }
      
      p = ivect_iter->world_p;
      tp = ivect_iter->t;
    }
    
    if (tp != t1 && inside_coll.size() != 0) {
      double len = ivect_iter->t - tp, weight = len/(double)NSTEPS;
      color voltrans = black;
      
      for (object_collection::iterator obj_iter = inside_coll.begin(); obj_iter != inside_coll.end(); obj_iter++) {
	voltrans += obj_iter->_volumeshader->filter(p, ivect_iter->world_p, r.t, NSTEPS);
      }
      
      trans *= (weight*voltrans).exp();
    }
    
    return trans;
  }
  

  color raytrace(const ray& r, scene& world, int maxdepth) {
    static int depth = 0;

    if (depth > maxdepth) {
      return tracer::black;
    }

    depth++;

    object_collection inside_coll;
    intersection_vector ivect;
    
    // find intersections and check if we're inside objects
    for (object_collection::iterator obj_iter = world.objects.begin(); obj_iter != world.objects.end(); obj_iter++) {
      obj_iter->intersect(r, ivect);
      if (obj_iter->inside(r.p, r.t) && obj_iter->_volumeshader != NULL) inside_coll.insert(inside_coll.begin(), *obj_iter);
    }
    
    std::sort(ivect.begin(), ivect.end());
    
    color c(0,0,0), trans(1,1,1);
    vector p = r.p;
    double tp = 0;
    // iterate over all intersections
    for (intersection_vector::iterator ivect_iter = ivect.begin(); ivect_iter != ivect.end(); ivect_iter++) {
      shade_context shtx(r.v, &world, *ivect_iter);
      
#define TRACESTEPS 10
      if (inside_coll.size() != 0) {	
	double len = ivect_iter->t - tp, weight = len/(double)TRACESTEPS;
	color vollight = black, volfilter = black;
	
	for (int i=0;i<TRACESTEPS;i++) {
	  vector currentpoint = p + (i/(double)TRACESTEPS)*(ivect_iter->world_p - p);
	  
	  for (object_collection::iterator obj_iter = inside_coll.begin(); obj_iter != inside_coll.end(); obj_iter++) {
	    volumeshader::volume_element elem = obj_iter->_volumeshader->shade(r, shade_context(currentpoint, vector(0,0,0), r.v, &world, r.t), r.t);
	    color f = elem.filter;
	    
	    for (lightsource_collection::iterator light_iter  = world.lightsources.begin(); light_iter != world.lightsources.end(); light_iter++) {
	      ray rayoflight = (*light_iter)->shade(currentpoint, r.t);
	      rayoflight.c *= filter(rayoflight, 0, (rayoflight.p - currentpoint).length(), world);
	      
	      //vollight += rayoflight.c*solidness*((weight*voltrans).exp());
	      vollight += elem.alpha*elem.c*rayoflight.c*(weight*volfilter).exp();
	    }
	    
	    volfilter += weight*f;
	  }
	}
	
	trans *= (len*volfilter).exp();
	c += vollight;
      }
      
      // add ambient light
      c += ivect_iter->obj->_shader->ambient(shtx);

      // iterate over all lightsources
      for (lightsource_collection::iterator light_iter = world.lightsources.begin(); light_iter != world.lightsources.end(); light_iter++) {
	// create ray
	ray rayoflight = (*light_iter)->shade(shtx.P, r.t);
	// filter light
	rayoflight.c *= filter(rayoflight, 0, (shtx.P - rayoflight.p).length(), world);
	
	if (rayoflight.c.value() != 0.0) c += trans*ivect_iter->obj->_shader->shade(rayoflight, shtx);
      }
/*      
      // add global illumination
      if (maxglobaldepth > 0) {
	c += trans*illumination(ivect_iter->p, ivect_iter->n, r.v, world, r.t, maxdepth, maxglobaldepth);
      }
  */    
      trans *= ivect_iter->obj->_shader->transparency(shtx);
      
      if (trans.value() == 0) break;
      
      if (ivect_iter->obj->_volumeshader != NULL) {
	if (ivect_iter->type == intersection::ENTER) {
	  inside_coll.insert(inside_coll.begin(), *ivect_iter->obj);
	} else {
	  inside_coll.remove(*(ivect_iter->obj));		
	}
      }
      
      p = ivect_iter->world_p;
      tp = ivect_iter->t;
    }
    
    depth--;

    return c + 0.25*tracer::color(0.6, 0.65, 0.8)*trans;
    //return c;
  }
}

color ambient(const vector& p, const vector& n, scene& world, int angle, int nsamples, t) {
  double x = acos(n*vector(0,1,0))/2.0 + 0.5;

  return color(x,x,x);
}
