#include "fog.h"

Fog::Fog() : tracer::volumeshader("Fog") {
}
Fog::~Fog() {
}

tracer::volumeshader::volume_element Fog::shade(const tracer::ray& r, const tracer::shade_context& context) {
  return tracer::volumeshader::volume_element(tracer::white, tracer::opaque, 1.0);
}
std::ostream& Fog::toStream(std::ostream& out) const {
  return out;
}

std::istream& Fog::fromStream(std::istream& in) {
  while (in.get() != '}');

  return in;
}

tracer::volumeshader* Fog::clone() {
  return new Fog();
}

