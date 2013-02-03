#ifndef __PLANETSHADER_H__
#define __PLANETSHADER_H__

#include "../tracer.h"

class PlanetShader : public tracer::shader {
public:
  // constructors and destructors
  PlanetShader();
  // methods
  tracer::color earth(const tracer::ray& r, const tracer::shade_context& context);
  tracer::color water(const tracer::ray& r, const tracer::shade_context& context);
  tracer::color clouds(const tracer::ray& r, const tracer::shade_context& context);
  tracer::color caps(const tracer::ray& r, const tracer::shade_context& context);
  tracer::vector normal(const tracer::ray& r, const tracer::shade_context& context);
  virtual tracer::color shade(const tracer::ray& r, const tracer::shade_context& context);
  virtual std::ostream& toStream(std::ostream& out) const;
  virtual std::istream& fromStream(std::istream& in);
  virtual tracer::shader* clone();
protected:
  // data members
};

#endif
