#ifndef __FUNCTION_FACTORY__
#define __FUNCTION_FACTORY__

#include "function.h"
#include "linear_spline.h"

class function_factory {
  function* createFunction(std::istream& in) {
    function *ret = NULL;

    // 1. determine type
    std::string type;

    in >> type;

    if (type == "linear_spline<T>") {
      linear_spline<
    }

    return ret;
  }
};

#endif
