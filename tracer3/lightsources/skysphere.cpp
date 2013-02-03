#include "skysphere.h"

Skysphere::Skysphere() : tracer::lightsource("Skysphere") {
}

tracer::ray Skysphere::shade(const tracer::vector& p, double t) {
  tracer::vector dx((rand()%2000-1000)/1000.0, (rand()%2000-1000)/1000.0, (rand()%2000-1000)/1000.0);
  
  while (dx.length2() > 1.0) dx = tracer::vector((rand()%2000-1000)/1000.0, (rand()%2000-100)/1000.0, (rand()%2000-1000)/1000.0);
  
  dx.y = fabs(dx.y);

  return tracer::ray(p + (dx^100), -(dx^1), t, c);
}

int Skysphere::inside(const tracer::vector& p, double t) {
  return 1;
}

std::ostream& Skysphere::toStream(std::ostream& out) const {
  out << " color " << c;

  return out;
}

std::istream& Skysphere::fromStream(std::istream& in) {
  std::string tmp = "";
  
  while (tmp != "}") {
    in >> tmp;
    
    if (tmp == "color") {
      in >> c;
    }
  }
  
  return in;
}

tracer::lightsource* Skysphere::clone() {
  return new Skysphere();
}

