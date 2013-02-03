#ifndef __NORMALSHADER_H__
#define __NORMALSHADER_H__

#include "../tracer.h"

class NormalShader : public tracer::shader {
public:
  // constructors and destructors
  NormalShader();
  // methods
  virtual tracer::color ambient(const tracer::shade_context& context);
  virtual tracer::color shade(const tracer::ray& r, const tracer::shade_context& context);
  virtual std::ostream& toStream(std::ostream& out) const;
  virtual std::istream& fromStream(std::istream& in);
  virtual tracer::shader* clone();
protected:
  // data members
};

#endif
