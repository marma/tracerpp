#include "planetshader.h"

PlanetShader::PlanetShader() : tracer::shader("PlanetShader") {
}

tracer::color PlanetShader::earth(const tracer::ray& r, const tracer::shade_context& context) {
}

tracer::color PlanetShader::water(const tracer::ray& r, const tracer::shade_context& context) {
}

tracer::color PlanetShader::clouds(const tracer::ray& r, const tracer::shade_context& context) {
}

tracer::color PlanetShader::caps(const tracer::ray& r, const tracer::shade_context& context) {
}

tracer::vector PlanetShader::normal(const tracer::ray& r, const tracer::shade_context& context) {
  return context.N;
}

tracer::color PlanetShader::shade(const tracer::ray& r, const tracer::shade_context& context) {
  double H = 2 + tracer::noise::perlin::noise(r.p), x = tracer::noise::perlin::VLfBm(r.p*5, );
}

std::ostream& PlanetShader::toStream(std::ostream& out) const {
  return out;
}

std::istream& PlanetShader::fromStream(std::istream& in) {
  while (in.get() != '}');

  return in;
}

tracer::shader* PlanetShader::clone() {
  return new PlanetShader();
}

