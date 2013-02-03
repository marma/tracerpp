#ifndef __CHECKERSHADER_H__
#define __CHECKERSHADER_H__

#include <iostream>
#include "../shader.h"

class CheckerShader : public tracer::shader {
 public:
  CheckerShader();
  virtual std::ostream& toStream(std::ostream& out) const;
  virtual std::istream& fromStream(std::istream& in);
  virtual tracer::color shade(const tracer::ray& r, const tracer::shade_context& context);
  virtual tracer::shader* clone();
 protected:
  double Ka, Kd, Kp, Km, phong_size,scale;
  tracer::color c1, c2;
};

#endif
