#include "glassshader.h"

GlassShader::GlassShader() : tracer::shader("GlassShader"), ior(1.0) {
}

tracer::color GlassShader::ambient(const tracer::shade_context& context) {
  tracer::vector I = -context.E, N = (context.N*I < 0)? -context.N:context.N, T, p = context.P;
  double n = (context.N*I > 0)? 1.0/ior:ior;

  T = (n*(N*I) - sqrt(1 - n*n*(1 - (N*I)*(N*I))))*N - n*I;

  p += (T*N > 0)? 0.0001*N:-0.0001*N;

  return tracer::raytrace(tracer::ray(p, T^1, context.time), *context.world);  
}

tracer::color GlassShader::shade(const tracer::ray& r, const tracer::shade_context& context) {
  tracer::color specular = tracer::black;
  double d = -context.N * r.v;
  
  d = fabs(d);
  
  if (d>0) { 
    // specular
    tracer::vector N = context.N;
    tracer::vector L = r.v;
    tracer::vector V = context.E;
    tracer::vector R = (2*(N * L)*N - L)^1.0;
    
    specular = Kp*pow(R * V, phong_size)*r.c;
  }
  
  return specular;
}

std::ostream& GlassShader::toStream(std::ostream& out) const {
  return out;
}

std::istream& GlassShader::fromStream(std::istream& in) {
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
    } else if (tmp == "ior") {
      in >> ior;
    }
  }
  
  return in;
}

tracer::shader* GlassShader::clone() {
  return new GlassShader();
}

