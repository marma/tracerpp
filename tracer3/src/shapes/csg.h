#ifndef __CSG_H__
#define __CSG_H__

#include <vector>
#include <algorithm>
#include "../tracer.h"

class CSG : public tracer::shape {
public:
  enum { UNION, DIFFERENCE };
  // constructors and destructors
  CSG();
  ~CSG();
  // methods
  virtual void intersect(const tracer::ray& r, tracer::intersection_collection& icoll);
  virtual int inside(const tracer::vector& p, double t);
  virtual std::ostream& toStream(std::ostream& out) const;
  virtual std::istream& fromStream(std::istream& in);
  virtual tracer::shape* clone();
  void setOperator(int _op);
  void addShape(tracer::shape *_sh);
protected:
  std::vector<tracer::object*> objects;
  int op;
};

#endif
