#ifndef __VECTOR_SPLINE_H__
#define __VECTOR_SPLINE_H__

#include "spline.h"
#include "vector_function.h"

template<> class spline<vector> : public vector_function<double> {
 public:
  virtual vector operator() (const double& x) =0;
};

typedef spline<vector> vector_spline;

#endif

