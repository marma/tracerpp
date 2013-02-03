#include "noiseshader.h"

NoiseShader::NoiseShader() : tracer::shader("NoiseShader"), d(1), Ka(0.0), Kd(1.0), Kp(1.0), phong_size(20), scale(1), H(2.2), lacunarity(2), bump_size(0.1), octaves(4), type(NOISE), bump(0) {
  double l = 1;
  max = 0;

  for (int i=0;i<octaves;i++) {
    max += pow(l, -H);
    l *= lacunarity;
  }
}

double NoiseShader::value(const tracer::vector& p) {
  switch (type) {
    case NOISE: return 0.5*(1.0 + tracer::noise::perlin::noise(p));
    case FBM: return 1.0/(2 * max) * (max + tracer::noise::perlin::fBm(p, H, lacunarity, octaves));
    case VLNOISE: return 0.5*(1.0 + tracer::noise::perlin::VLnoise(p, d));
    case VLFBM: return 1.0/(2 * max) * (max + tracer::noise::perlin::VLfBm(p, d, H, lacunarity, octaves));
  }

  return 0;
}

tracer::vector NoiseShader::normal(const tracer::vector& p, double dx) {
  switch (type) {
    case NOISE: return tracer::noise::perlin::dnoise(p, dx);
    case FBM: return tracer::noise::perlin::dfBm(p, H, lacunarity, octaves, dx);
    case VLNOISE: return tracer::noise::perlin::dVLnoise(p, d, dx);
    case VLFBM: return tracer::noise::perlin::dVLfBm(p, d, H, lacunarity, octaves, dx);
  }

  return tracer::vector(0,1,0);
}

tracer::color NoiseShader::shade(const tracer::ray& r, const tracer::shade_context& context) {
	tracer::color diffuse = tracer::black, specular = tracer::black, c;
	tracer::vector N = context.N;
	double x = value(scale*context.P);

	c = x*c2 + (1.0 - x)*c1;

	if (bump) {
	  N = (N + bump_size*normal(scale*context.P))^1;
	}

	double d = -N*r.v;
	if (d>0) { 
	  // diffuse
	  diffuse = Kd*d*r.c*c;
	  
	  // specular
	  tracer::vector L = r.v;
	  tracer::vector V = context.E;
	  tracer::vector R = 2*(N * L)*N - L;
	  specular = Kp*pow(R * V, phong_size)*r.c;
	}
	
	return diffuse + specular;
}

tracer::color NoiseShader::transparency(const tracer::shade_context& context) {
  return tracer::black;
}

std::ostream& NoiseShader::toStream(std::ostream& out) const {
  return out << "Ka " << Ka << " Kd " << Kd << " Kp " << Kp << " phong_size " << phong_size << " color1 " << c1 << " color2 " << c2 << " H " << H << " octaves " << octaves << " lacunarity " << lacunarity;
}

std::istream& NoiseShader::fromStream(std::istream& in) {
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
		} else if (tmp == "color1") {
			in >> c1;
		} else if (tmp == "color2") {
			in >> c2;
		} else if (tmp == "scale") {
		        in >> scale;
		} else if (tmp == "H") {
			in >> H;
		} else if (tmp == "d") {
			in >> d;
		} else if (tmp == "lacunarity") {
			in >> lacunarity;
		} else if (tmp == "octaves") {
			in >> octaves;
		} else if (tmp == "bump_size") {
			in >> bump_size;
		} else if (tmp == "bump") {
   		        bump = 1;
		} else if (tmp == "type") {
                        in >> tmp;
                        
                        if (tmp == "NOISE") {
                          type = NOISE;
                        } else if (tmp == "FBM") {
			  type = FBM;
                        } else if (tmp == "VLNOISE") {
			  type = VLNOISE;
                        } else if (tmp == "VLFBM") {
			  type = VLFBM;
			}
                }
	}

	return in;
}

tracer::shader* NoiseShader::clone() {
	return new NoiseShader();
}

