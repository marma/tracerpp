#include "checkershader.h"

CheckerShader::CheckerShader() : tracer::shader("CheckerShader"), scale(1.0) {
}

tracer::color CheckerShader::shade(const tracer::ray& r, const tracer::shade_context& context) {
  tracer::color diffuse = tracer::black, specular = tracer::black;
  double d = -context.N * r.v;
  
  d = fabs(d);
  
  if (d>0) { 
    // diffuse
    diffuse = ( ((int)fabs(floor(scale*context.P.x)) % 2) == ((int)fabs(floor(scale*context.P.z + 2*context.time)) % 2) )? Kd*d*r.c*c1 : Kd*d*r.c*c2;
    
    // specular
    if (Kp != 0.0) {
      tracer::vector N = context.N;
      tracer::vector L = r.v;
      tracer::vector V = context.E;
      tracer::vector R = (2*(N * L)*N - L)^1.0;
    
      specular = Kp*pow(R * V, phong_size)*r.c;
    }
  }
  
  return diffuse + specular;
}

tracer::shader* CheckerShader::clone() {
  return new CheckerShader();
}

std::ostream& CheckerShader::toStream(std::ostream& out) const {
  return out;
}

std::istream& CheckerShader::fromStream(std::istream& in) {
  std::string tmp = "";
  
  while (tmp != "}") {
    in >> tmp;
    
    if (tmp == "Ka") {
      in >> Ka;
    } else if (tmp == "Kd") {
      in >> Kd;
    } else if (tmp == "Kp") {
      in >> Kp;
    } else if (tmp == "phong_size") {
      in >> phong_size;
    } else if (tmp == "scale") {
      in >> scale;
    } else if (tmp == "color1") {
      in >> c1;
    } else if (tmp == "color2") {
      in >> c2;
    }
  }
  
  return in;
}
