#ifndef __COLOR_SPLINE_H__
#define __COLOR_SPLINE_H__

#include "color.h"
#include "hermite_spline.h"

namespace tracer {
  typedef hermite_spline<color> color_spline;
}
