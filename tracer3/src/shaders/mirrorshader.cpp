#include "mirrorshader.h"

MirrorShader::MirrorShader() : tracer::shader("MirrorShader") {
}

tracer::color MirrorShader::ambient(const tracer::shade_context& context) {
  tracer::vector I = context.E - 2*(context.E*(context.world_N))*context.world_N;

  return Km*tracer::raytrace(tracer::ray(context.world_P + 0.00001*context.world_N, I, context.time), *context.world);
}

tracer::color MirrorShader::shade(const tracer::ray& r, const tracer::shade_context& context) {
  tracer::color diffuse = tracer::black, specular = tracer::black;
  double d = -context.N * r.v;
  
  d = fabs(d);
  
  if (d>0) { 
    // diffuse
    diffuse = d*r.c*c;
    
    // specular
    if (Kp != 0.0) {
      tracer::vector N = context.N;
      tracer::vector L = r.v;
      tracer::vector V = context.E;
      tracer::vector R = (2*(N * L)*N - L)^1.0;
    
      specular = Kp*pow(R * V, phong_size)*r.c;
    }
  }
  
  return Kd*diffuse + Kp*specular;
}

tracer::shader* MirrorShader::clone() {
  return new MirrorShader();
}

std::ostream& MirrorShader::toStream(std::ostream& out) const {
  return out;
}

std::istream& MirrorShader::fromStream(std::istream& in) {
  std::string tmp = "";
  
  while (tmp != "}") {
    in >> tmp;
    
    if (tmp == "Ka") {
      in >> Ka;
    } else if (tmp == "Kd") {
      in >> Kd;
    } else if (tmp == "Kp") {
      in >> Kp;
    } else if (tmp == "Km") {
      in >> Km;
    } else if (tmp == "phong_size") {
      in >> phong_size;
    } else if (tmp == "color") {
      in >> c;
    }
  }
  
  return in;
}
