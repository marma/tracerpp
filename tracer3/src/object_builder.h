#ifndef __OBJECT_BUILDER_H__
#define __OBJECT_BUILDER_H__

#include <string>
#include "object.h"
#include "shape_factory.h"
#include "shader_factory.h"
#include "vector_function_factory.h"

namespace tracer {
  class object_builder {
  public:
    // constructors and destructors
    
    // methods
    static object* buildObject(std::istream& in) {
      shape *shp = NULL;
      shader *shd = NULL;
      volumeshader *vshd = NULL;
      vector_function *tr = NULL;
      std::string tmp;
      
      while (tmp != "}") {
	if (tmp == "shape") {
	  shp = shape_factory::createShape(in);
	} else if (tmp == "shader") {
	  shd = shader_factory::createShader(in);
	} else if (tmp == "volumeshader") {
	  //vshd = volumeshader_factory::createVolumeShader(in);
	} else if (tmp == "translation") {
	  tr = vector_function_factory::createVectorFunction(in);
	} else if (tmp == "rotation") {
	}
	
	in >> tmp;
      }
      
      return new object(shp, shd, vshd, tr);
    }
    
    // data members
    // friends
  };
}

#endif
