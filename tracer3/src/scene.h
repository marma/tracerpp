#ifndef __SCENE_H__
#define __SCENE_H__

#include "vector.h"
#include "shape.h"
#include "volumeshader.h"
#include "lightsource.h"
#include "object.h"

namespace tracer {
  class scene {
  public:
    void add(lightsource *light) {
      lightsources.insert(lightsources.begin(), light);
    }
    
    void add(shape *_shape, shader *_shader, volumeshader *_vshader = NULL, vector_function *_tr = NULL) {
      objects.insert(objects.begin(), object(_shape, _shader, _vshader, _tr));
    }

    object_collection::iterator getLigthsources(const vector& p, double& t);
    
    object_collection objects;
    lightsource_collection lightsources;
  };
}

#endif
