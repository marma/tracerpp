#ifndef __MIRRORSHADER_H__
#define __MIRRORSHADER_H__

#include <iostream>
#include "../shader.h"
#include "../raytrace.h"
#include "../noise.h"

class MirrorShader : public tracer::shader {
 public:
  MirrorShader();
  virtual std::ostream& toStream(std::ostream& out) const;
  virtual std::istream& fromStream(std::istream& in);
  virtual tracer::color ambient(const tracer::shade_context& context);
  virtual tracer::color shade(const tracer::ray& r, const tracer::shade_context& context);
  virtual tracer::shader* clone();
 protected:
  double Ka, Kd, Kp, Km, phong_size;
  tracer::color c;
};

#endif
