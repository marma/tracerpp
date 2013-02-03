#include "normalshader.h"

NormalShader::NormalShader() : tracer::shader("NormalShader") {
}

tracer::color NormalShader::ambient(const tracer::shade_context& context) {
  tracer::color ret(fabs(context.world_N.x), fabs(context.world_N.y), fabs(context.world_N.z));

  if (context.world_N.x < 0) {
    ret.r *= 0.5;
  }

  if (context.world_N.y < 0) {
    ret.g *= 0.5;
  }

  if (context.world_N.z < 0) {
    ret.b*= 0.5;
  }

  return ret;
}

tracer::color NormalShader::shade(const tracer::ray& r, const tracer::shade_context& context) {
  return tracer::black;
}

std::ostream& NormalShader::toStream(std::ostream& out) const {
  return out;
}

std::istream& NormalShader::fromStream(std::istream& in) {
  while (in.get() != '}');

  return in;
}

tracer::shader* NormalShader::clone() {
  return new NormalShader();
}

