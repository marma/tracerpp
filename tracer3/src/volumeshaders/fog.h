#ifndef __FOG_H__
#define __FOG_H__

#include "../tracer.h"

class Fog : public tracer::volumeshader {
public:
  // constructors and destructors
  Fog();
  virtual ~Fog();
  // methods
  virtual tracer::volumeshader::volume_element shade(const tracer::ray& r, const tracer::shade_context& context);
  virtual std::ostream& toStream(std::ostream& out) const;
  virtual std::istream& fromStream(std::istream& in);
  virtual tracer::volumeshader* clone();
protected:
  // data members
};

#endif
