#ifndef __FLATSHADER_H__
#define __FLATSHADER_H__

#include "../tracer.h"

class FlatShader : public tracer::shader {
public:
  // constructors and destructors
  FlatShader();
  // methods
  virtual tracer::color shade(const tracer::ray& r, const tracer::shade_context& context);
  virtual std::ostream& toStream(std::ostream& out) const;
  virtual std::istream& fromStream(std::istream& in);
  virtual tracer::shader* clone();
protected:
  // data members
};

#endif
