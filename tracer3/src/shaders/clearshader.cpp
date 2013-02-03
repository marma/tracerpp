#include "clearshader.h"

ClearShader::ClearShader() : tracer::shader("ClearShader") {
}

tracer::color ClearShader::shade(const tracer::ray& ray, const tracer::shade_context& context) {
  return tracer::black;
}

tracer::color ClearShader::transparency(const tracer::shade_context& context) {
  return tracer::opaque;
}

std::ostream& ClearShader::toStream(std::ostream& out) const {
  return out;
}

std::istream& ClearShader::fromStream(std::istream& in) {
  while (in.get() != '}');

  return in;
}

tracer::shader* ClearShader::clone() {
  return new ClearShader();
}

