#ifndef __VECTOR_FUNCTION_FACTORY_H__
#define __VECTOR_FUNCTION_FACTORY_H__

#include "vector_function.h"
#include "vector_spline.h"

namespace tracer {
  class vector_function_factory {
  public:
    static vector_function* createVectorFunction(std::istream& in) {
      std::string name;
      vector_function *f = NULL;
      vector_spline *hs = NULL;
      
      in >> name;
      
      if (name == "hermite_spline") {
	hs = new vector_spline;
	f = hs;
	
	in >> *hs;
      }
      
      //std::cerr << "returning " << f << " -> " << *hs << std::endl;
      
      return f;
    }
  };
}

#endif
