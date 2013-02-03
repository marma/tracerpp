#ifndef __SHAPE_FACTORY_H__
#define __SHAPE_FACTORY_H__

#include <map>
#include <string>
#include "shape.h"

namespace tracer {
  class shape_factory {
  public:
    // contructors & destructors
    // methods
    static int registerShape(shape *s) {
      //std::cerr << "Registering shape '" << s->__name << "'" << std::endl;
      
      prototypes[s->__name] = s;
      
      return 1;
    }
    
    static shape* createShape(std::istream& in) {
      std::string name;
      
      in >> name;
      
      shape *s = prototypes[name]->clone();
      
      in >> *s;
      
      //std::cerr << "Created shape '" << *s << "'" << std::endl;
      
      return s;
    }
    
    // data members
    static std::map<std::string, shape*> prototypes;
    // friends
  };
}

#endif
