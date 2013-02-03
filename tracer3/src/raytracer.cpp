#include "raytracer.h"

namespace tracer {
  raytracer::raytracer() :serializable("raytracer") {
  }

  color raytracer::filter(ray& r, double t0, double t1, scene& world) {
    intersection_vector ivect;

    // find intersections
    for (object_collection::iterator obj_iter = world.objects.begin(); obj_iter != world.objects.end(); obj_iter++) {
      obj_iter->intersect(r, ivect);
    }

    color trans = tracer::white;

    // iterate over all intersections
    intersection_vector::iterator ivect_iter;
    for (ivect_iter = ivect.begin(); ivect_iter != ivect.end() && trans != tracer::black; ivect_iter++) {
      if (ivect_iter->t >= t0 && ivect_iter->t <= t1-0.001) {
        shade_context shtx(r.v, &world, *ivect_iter);

        trans *= ivect_iter->obj->_shader->transparency(shtx);
      }
    }

    return trans;
  }

  color raytracer::raytrace(const ray& r, scene& world, int maxdepth) {
    static int depth = 0;

    if (depth > maxdepth) {
      return tracer::black;
    }

    depth++;

    intersection_vector ivect;

    // find intersections
    for (object_collection::iterator obj_iter = world.objects.begin(); obj_iter != world.objects.end(); obj_iter++) {
      obj_iter->intersect(r, ivect);
    }

    std::sort(ivect.begin(), ivect.end());

    color c(0,0,0), trans(1,1,1);
    // iterate over all intersections
    for (intersection_vector::iterator ivect_iter = ivect.begin(); ivect_iter != ivect.end() && trans != tracer::black; ivect_iter++) {
      shade_context shtx(r.v, &world, *ivect_iter);

      // add ambient light
      c += ivect_iter->obj->_shader->ambient(shtx);

      // iterate over all lightsources
      for (lightsource_collection::iterator light_iter = world.lightsources.begin(); light_iter != world.lightsources.end(); light_iter++) {
        // create ray
        ray rayoflight = (*light_iter)->shade(shtx.world_P, r.t);
        // filter light
        rayoflight.c *= filter(rayoflight, 0, (shtx.world_P - rayoflight.p).length(), world);

        if (rayoflight.c != tracer::black) c += trans*ivect_iter->obj->_shader->shade(rayoflight, shtx);
      }

      trans *= ivect_iter->obj->_shader->transparency(shtx);
    }

    depth--;

    return c + 0.25*tracer::color(0.6, 0.65, 0.8)*trans;
  }

  color raytracer::ambient(const vector& p, const vector& n, scene& world, double angle, int nsamples, double t) {
    color trans = tracer::black;
    double x = (acos(-n*vector(0,1,0))/PI + 1.0)/2;

    for (int i=0;i<nsamples;i++) {
      tracer::vector dx((rand()%2000-1000)/1000.0, (rand()%2000-1000)/1000.0, (rand()%2000-1000)/1000.0);

      while (dx.length2() > 1.0) dx = tracer::vector((rand()%2000-1000)/1000.0, (rand()%2000-100)/1000.0, (rand()%2000-1000)/1000.0);

      dx.y = fabs(dx.y);

      dx = dx^1;

      ray r(p + 100*dx, -dx, t);

      trans += filter(r, 0, 99.99999, world);
    }

    return (nsamples == 0)? x*tracer::white:x*trans/nsamples;   
  }

  std::ostream& raytracer::toStream(std::ostream& out) const {
    out << "nsamples " << nAmbientSamples << " camera " << c << " imager " << i << " ";
  }

  std::istream& raytracer::fromStream(std::istream& in) {
    std::string token;

    in >> token;

    while (token != "}") {
      if (token == "camera") {
        in >> c;
      } else if (token == "imager") {
	in >> i;
      } else if (token == "scene") {
	s = tracer::scene_builder::buildScene(in);
      } else if (token == "ambient_samples") {
	in >> nAmbientSamples;
      }
    }
  }
}
