#include "csg.h"

CSG::CSG() : tracer::shape("CSG") {
}

CSG::~CSG() {
}

void CSG::intersect(const tracer::ray& r, tracer::intersection_collection& icoll) {
  tracer::intersection_vector iv;
  int n = (inside(r.p, r.t))? 1:0;

  for (std::vector<tracer::object*>::iterator oiter = objects.begin();oiter != objects.end();oiter++) {
    (*oiter)->intersect(r, iv);
  }
  
  std::sort(iv.begin(), iv.end());


  for (tracer::intersection_vector::iterator iiter = iv.begin();iiter != iv.end();iiter++) {
    if (op == UNION || iiter->obj == objects[0]) {
      if (iiter->type == tracer::intersection::ENTER) {
	if (++n == 1) {
	  icoll.insert(icoll.end(), *iiter);
	}
      } else if (iiter->type == tracer::intersection::EXIT) {
	if (--n == 0) {
	  icoll.insert(icoll.end(), *iiter);
	}
      }
    } else if (op == DIFFERENCE) {
      if (iiter->type == tracer::intersection::ENTER) {
	if (--n == 1) {
	  iiter->type = tracer::intersection::EXIT;

	  if (iiter->model_n*r.v > 0) {
	    iiter->world_n *= -1;
	    iiter->model_n *= -1;
	  }

	  icoll.insert(icoll.end(), *iiter);
	}
      } else if (iiter->type == tracer::intersection::EXIT) {
	if (++n == 1) {
	  iiter->type = tracer::intersection::ENTER;
	  
          if (iiter->model_n*r.v > 0) {
            iiter->world_n *= -1;
            iiter->model_n *= -1;
          }

	  icoll.insert(icoll.end(), *iiter);
	}
      }
    }
  }
}

int CSG::inside(const tracer::vector& p, double t) {
  int n=0;

  if (op == UNION) {
    for (std::vector<tracer::object*>::iterator oiter = objects.begin();oiter != objects.end();oiter++) {
      if ((*oiter)->inside(p, t)) {
	return 1;
      }
    }
  } else if (op == DIFFERENCE) {
    for (std::vector<tracer::object*>::iterator oiter = objects.begin();oiter != objects.end();oiter++) {
      if ((*oiter)->inside(p, t)) {
	if ((*oiter) == objects[0]) {
	  n++;
	} else {
	  n--;
	}
      }
    }
   
    return (n>0)? 1:0;
  }

  return 0;
}

std::ostream& CSG::toStream(std::ostream& out) const {
  return out;
}

std::istream& CSG::fromStream(std::istream& in) {
  std::string tmp = "";
  
  while (tmp != "}") {
    in >> tmp;
    
    if (tmp == "type") {
      in >> tmp;

      if (tmp == "UNION") op = UNION;
      if (tmp == "DIFFERENCE") op = DIFFERENCE;
     } else if (tmp == "object") {
      tracer::object *o = tracer::object_builder::buildObject(in);
      objects.insert(objects.end(), o);
    }
  }
  
  return in;
}

tracer::shape* CSG::clone() {
  return new CSG();
}

