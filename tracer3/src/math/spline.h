#ifndef __SPLINE_H__
#define __SPLINE_H__

#include "function.h"

template<class T> class spline : public function<T, double> {
 public:
  virtual T operator() (const double& x) =0;
};

#endif
