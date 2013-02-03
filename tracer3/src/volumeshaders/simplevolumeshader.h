#ifndef __SIMPLEVOLUMESHADER_H__
#define __SIMPLEVOLUMESHADER_H__

#include "../tracer.h"
#include "../noise.h"
#include "../color.h"

class SimpleVolumeShader : public tracer::volumeshader {
public:
  SimpleVolumeShader(tracer::vector _o = tracer::vector(0,0,0), double _r = 1.0);
  double mask1(const tracer::vector p) const;
  virtual tracer::volumeshader::volume_element shade(const tracer::ray& ra, const tracer::shade_context& context) const;
  virtual tracer::color filter(tracer::vector& p, double t) const;
  virtual std::ostream& toStream(std::ostream& out) const;
  virtual std::istream& fromStream(std::istream& in);
  virtual tracer::volumeshader* clone();

  tracer::vector o;
  double r;       
};

#endif
