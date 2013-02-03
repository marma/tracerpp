#include "flatshader.h"

FlatShader::FlatShader() : tracer::shader("FlatShader") {
}

tracer::color FlatShader::shade(const tracer::ray& r, const tracer::shade_context& context) {
  return r.c;
}
std::ostream& FlatShader::toStream(std::ostream& out) const {
  return out;
}

std::istream& FlatShader::fromStream(std::istream& in) {
  while (in.get() != '}');

  return in;
}

tracer::shader* FlatShader::clone() {
  return new FlatShader();
}

