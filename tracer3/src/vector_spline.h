#ifndef __VECTOR_SPLINE_H__
#define __VECTOR_SPLINE_H__

#include "vector.h"
#include "hermitespline.h"

namespace tracer {
  typedef hermite_spline<vector> vector_spline;
}

#endif
