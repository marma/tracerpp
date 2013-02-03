#ifndef __TEST_H__
#define __TEST_H__

#include "../tracer.h"

class Test : public tracer::shader {
public:
  // constructors and destructors
  Test();
  // methods
  virtual tracer::color shade(const tracer::ray& ray, const tracer::shade_context& context);
  virtual std::ostream& toStream(std::ostream& out) const;
  virtual std::istream& fromStream(std::istream& in);
  virtual tracer::shader* clone();
protected:
  // data members
};

#endif
