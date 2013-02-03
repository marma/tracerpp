#ifndef __AABOX_H__
#define __AABOX_H__

#include "../tracer.h"

class AABox : public tracer::shape {
public:
  // constructors and destructors
  AABox();
  // methods
  virtual void intersect(const tracer::ray& r, tracer::intersection_collection& icoll);
  virtual int inside(const tracer::vector& p, double t);
  virtual std::ostream& toStream(std::ostream& out) const;
  virtual std::istream& fromStream(std::istream& in);
  virtual tracer::shape* clone();
protected:
  // data members
  tracer::vector corner1, corner2;
};

#endif
