#ifndef __SKYLIGHTSOURCE_H__
#define __SKYLIGHTSOURCE_H__

#include "../tracer.h"

class SkyLightsource : public tracer::lightsource {
public:
  // constructors and destructors
  SkyLightsource();
  // methods
  virtual tracer::ray shade(const tracer::vector& p, double t);
  virtual int inside(const tracer::vector& p, double t);
  virtual std::ostream& toStream(std::ostream& out) const;
  virtual std::istream& fromStream(std::istream& in);
  virtual tracer::lightsource* clone();
protected:
  // data members
};

#endif
