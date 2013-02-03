#ifndef __SCENE_BUILDER_H__
#define __SCENE_BUILDER_H__

#include "object_builder.h"
#include "lightsource_factory.h"

namespace tracer {
  class scene_builder {
  public:
    // constructors and destructors
    // methods
    static scene* buildScene(std::istream& in) {
      std::string tmp;
      scene *s = new scene();
      
      while (in) {
	if (tmp == "lightsource") {
	  s->add(lightsource_factory::createLightsource(in));

	  //	  std::cerr << "adding light" << std::endl;
	} else if (tmp == "object") {
	  object *o = object_builder::buildObject(in);
	  
	  s->add(o->_shape, o->_shader, o->_volumeshader, o->_translation);

	  //	  std::cerr << "adding object" << std::endl;
	}
	
	in >> tmp;
      }
      
      return s;
    }
  };
}

#endif
