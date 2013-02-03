#ifndef __TRACER_H__
#define __TRACER_H__

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <list>
#include "vector.h"
#include "color.h"
#include "ray.h"
#include "intersection.h"
#include "shape.h"
#include "shader.h"
#include "lightsource.h"
#include "volumeshader.h"
#include "object.h"
#include "vector_function_factory.h"
#include "shape_factory.h"
#include "shader_factory.h"
#include "lightsource_factory.h"
#include "scene.h"
#include "scene_builder.h"

namespace tracer {
/*
	class volumeshader_factory {
	public:
		// contructors & destructors
		// methods
		static int registerVolumeShader(volumeshader *s) {
		  //std::cerr << "Registering shader '" << s->__name << "'" << std::endl;

			prototypes[s->__name] = s;

			return 1;
		}

 		static volumeshader* createVolumeShader(std::istream& in) {
 			std::string name;
 
	 		in >> name;
 
	 		volumeshader *s = prototypes[name]->clone();
 
	 		in >> *s;
		 	
			//std::cerr << "Created shader '" << *s << "'" << std::endl;

 			return s;
	 	}
 
		// data members
	 	static std::map<std::string, volumeshader*> prototypes;
		// friends
 	};
*/
}

#endif
