#ifndef __SHADER_FACTORY_H__
#define __SHADER_FACTORY_H__

#include <map>
#include <string>
#include "shader.h"

namespace tracer {
  class shader_factory {
  public:
    // contructors & destructors
    // methods
    static int registerShader(shader *s) {
      //std::cerr << "Registering shader '" << s->__name << "'" << std::endl;
      
      prototypes[s->__name] = s;
      
      return 1;
    }
    
    static shader* createShader(std::istream& in) {
      std::string name;
      
      in >> name;
      
      shader *s = prototypes[name]->clone();
      
      in >> *s;
      
      //std::cerr << "Created shader '" << *s << "'" << std::endl;
      
      return s;
    }
    
    // data members
    static std::map<std::string, shader*> prototypes;
    // friends
  };
}

#endif
