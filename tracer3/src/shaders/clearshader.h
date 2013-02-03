#ifndef __CLEARSHADER_H__
#define __CLEARSHADER_H__

#include "../tracer.h"

class ClearShader : public tracer::shader {
public:
  // constructors and destructors
  ClearShader();
  // methods
  virtual tracer::color shade(const tracer::ray& ray, const tracer::shade_context& context);
  virtual tracer::color transparency(const tracer::shade_context& context);
  virtual std::ostream& toStream(std::ostream& out) const;
  virtual std::istream& fromStream(std::istream& in);
  virtual tracer::shader* clone();
protected:
  // data members
};

#endif
