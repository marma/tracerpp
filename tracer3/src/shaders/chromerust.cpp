#include "chromerust.h"

ChromeRust::ChromeRust() : tracer::shader("ChromeRust"), d(1), Ka(0.0), Kd(1.0), Kp(1.0), phong_size(20), scale(1), H(2.2), lacunarity(2), bump_size(0.1), rust_level(0.5), octaves(4), type(NOISE), bump(0) {
  double l = 1;
  max = 0;

  for (int i=0;i<octaves;i++) {
    max += pow(l, -H);
    l *= lacunarity;
  }
}

double ChromeRust::value(const tracer::vector& p) {
  switch (type) {
    case NOISE: return 0.5*(1.0 + tracer::noise::perlin::noise(p));
    case FBM: return 1.0/(2 * max) * (max + tracer::noise::perlin::fBm(p, H, lacunarity, octaves));
    case VLNOISE: return 0.5*(1.0 + tracer::noise::perlin::VLnoise(p, d));
    case VLFBM: return 1.0/(2 * max) * (max + tracer::noise::perlin::VLfBm(p, d, H, lacunarity, octaves));
  }

  return 0;
}

tracer::vector ChromeRust::normal(const tracer::vector& p, double dx) {
  switch (type) {
    case NOISE: return tracer::noise::perlin::dnoise(p, dx);
    case FBM: return tracer::noise::perlin::dfBm(p, H, lacunarity, octaves, dx);
    case VLNOISE: return tracer::noise::perlin::dVLnoise(p, d, dx);
    case VLFBM: return tracer::noise::perlin::dVLfBm(p, d, H, lacunarity, octaves, dx);
  }

  return tracer::vector(0,1,0);
}

tracer::color ChromeRust::ambient(const tracer::shade_context& context) {
  double x = value(scale*context.P);

  if (x < rust_level) {
    tracer::vector I = context.E - 2*(context.E*(context.world_N))*context.world_N;

    return tracer::raytrace(tracer::ray(context.world_P + 0.00001*context.world_N, I, context.time), *context.world);
  } else {
    return tracer::black;
  }
}

tracer::color ChromeRust::shade(const tracer::ray& r, const tracer::shade_context& context) {
	tracer::color diffuse = tracer::black, specular = tracer::black, c;
	tracer::vector N = context.N;
	double x = value(scale*context.P);

	c = x*c2 + (1.0 - x)*c1;

	if (bump && x > rust_level) {
	  N = (N + bump_size*normal(scale*context.P))^1;
	}

	double d = -N*r.v;
	if (d>0) { 
	  // diffuse
	  diffuse = Kd*d*r.c*c;
	  
	  if (Kp != 0) {
	    // specular
	    tracer::vector L = r.v;
	    tracer::vector V = context.E;
	    tracer::vector R = 2*(N * L)*N - L;
	    specular = Kp*pow(R * V, phong_size)*r.c;
	  }
	}

	return (x > rust_level)? specular+diffuse:diffuse+specular;
}

tracer::color ChromeRust::transparency(const tracer::shade_context& context) {
  return tracer::black;
}

std::ostream& ChromeRust::toStream(std::ostream& out) const {
  return out << "Ka " << Ka << " Kd " << Kd << " Kp " << Kp << " phong_size " << phong_size << " color1 " << c1 << " color2 " << c2 << " H " << H << " octaves " << octaves << " lacunarity " << lacunarity;
}

std::istream& ChromeRust::fromStream(std::istream& in) {
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
		} else if (tmp == "rust_level") {
		  in >> rust_level;
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

tracer::shader* ChromeRust::clone() {
  return new ChromeRust();
}

