#include "skylightsource.h"

SkyLightsource::SkyLightsource() : tracer::lightsource("SkyLightsource") {
}

tracer::ray SkyLightsource::shade(const tracer::vector& p, double t) {
  //tracer::vector dx((rand()%2000-1000)/1000.0, 0, (rand()%2000-1000)/1000.0);
  
  //while (dx.length2() > 1.0) dx = tracer::vector((rand()%2000-1000)/1000.0, 0, (rand()%2000-1000)/1000.0);

  //dx.y = 20;

  return tracer::ray(p + tracer::vector(0,200,0), tracer::vector(0,-1,0), t, tracer::white);
}

int SkyLightsource::inside(const tracer::vector& p, double t) {
  return 1;
}

std::ostream& SkyLightsource::toStream(std::ostream& out) const {
  return out;
}

std::istream& SkyLightsource::fromStream(std::istream& in) {
  while (in.get() != '}');

  return in;
}

tracer::lightsource* SkyLightsource::clone() {
  return new SkyLightsource();
}

