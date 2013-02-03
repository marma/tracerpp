#include "simplevolumeshader.h"

SimpleVolumeShader::SimpleVolumeShader(tracer::vector _o, double _r) : tracer::volumeshader("SimpleVolumeShader"), o(_o), r(_r) {
}

double SimpleVolumeShader::mask1(const tracer::vector p) const {
  double dist = (p - o).length();
  
  if (dist > 0.5) {
    return 2*(1.0 - dist);
  } else {
    return 1;
  }
}

tracer::volumeshader::volume_element SimpleVolumeShader::shade(const tracer::ray& ra, const tracer::shade_context& context) const {
  tracer::vector x = context.P;
  tracer::color f = filter(x,ra.t);
  
  return tracer::volumeshader::volume_element(tracer::white, f, f.r/5.0);
}

tracer::color SimpleVolumeShader::filter(tracer::vector& p, double t) const {
  double dist = (p - o).length(), value;
  
  //value = 10*(tracer::noise::perlin::fBm(4*p + tracer::vector(1,2,3), 0.8, 1.8, 8));
  //return (value<0)? tracer::black : value*tracer::white;
  
  //value = 10*mask1(p)*(tracer::noise::perlin::fBm(2*p + tracer::vector(1,2,3), 0.8, 1.8, 8));
  value = 1;
  
  if (value < 0) {
    return tracer::black;
  }
  
  value *= 1.0 - pow((p - o).length(), 2);
  
  value += pow((dist - 1.0), 2);
  
  return tracer::color(value,value,value);
}

std::ostream& SimpleVolumeShader::toStream(std::ostream& out) const {
  return out;
}

std::istream& SimpleVolumeShader::fromStream(std::istream& in) {
  while (in.get() != '}');
  
  return in;
}

tracer::volumeshader* SimpleVolumeShader::clone() {
  return new SimpleVolumeShader();
}
