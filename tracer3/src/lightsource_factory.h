#ifndef __LIGHTSOURCE_FACTORY_H__
#define __LIGHTSOURCE_FACTORY_H__

#include <map>
#include <string>
#include "lightsource.h"

namespace tracer {
  class lightsource_factory {
  public:
    // contructors & destructors
    // methods
    static int registerLightsource(lightsource* l) {
      //std::cerr << "Registering lightsource '" << l->__name << "'" << std::endl;
      
      prototypes[l->__name] = l;
      
      return 1;
    }
    
    static lightsource* createLightsource(std::istream& in) {
      std::string name;
      
      in >> name;
      
      if (prototypes.find(name) == prototypes.end()) {
	std::cerr << "warning: no prototype for '" << name << "'" << std::endl;
      }

      lightsource *light = prototypes[name]->clone();
      
      in >> *light;
      
      //std::cerr << "Created lightsource '" << *light << "'" << std::endl;
      
      return light;
    }
    
    // data members
    static std::map<std::string, lightsource*> prototypes;
    // friends
  };
}

#endif
