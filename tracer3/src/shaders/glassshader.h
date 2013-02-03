#ifndef __GLASSSHADER_H__
#define __GLASSSHADER_H__

#include "../tracer.h"
#include "../raytrace.h"

class GlassShader : public tracer::shader {
public:
  // constructors and destructors
  GlassShader();
  // methods
  virtual tracer::color ambient(const tracer::shade_context& context);
  virtual tracer::color shade(const tracer::ray& ray, const tracer::shade_context& context);
  virtual std::ostream& toStream(std::ostream& out) const;
  virtual std::istream& fromStream(std::istream& in);
  virtual tracer::shader* clone();
protected:
  // data members
  double Ka, Kd, Kp, phong_size, ior;
};

#endif
