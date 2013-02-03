#ifndef __SKYSPHERE_H__
#define __SKYSPHERE_H__

#include "../tracer.h"

class Skysphere : public tracer::lightsource {
public:
  // constructors and destructors
  Skysphere();
  // methods
  virtual tracer::ray shade(const tracer::vector& p, double t);
  virtual int inside(const tracer::vector& p, double t);
  virtual std::ostream& toStream(std::ostream& out) const;
  virtual std::istream& fromStream(std::istream& in);
  virtual tracer::lightsource* clone();
protected:
  // data members
  tracer::color c;
};

#endif
