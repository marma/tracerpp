#ifndef __CHROMERUST_H__
#define __CHROMERUST_H__

#include "../tracer.h"
#include "../noise.h"
#include "../raytrace.h"

#define NOISE   0
#define FBM     1
#define VLNOISE 2
#define VLFBM   3

class ChromeRust : public tracer::shader {
public:
	ChromeRust();
	double value(const tracer::vector& p);
	tracer::vector normal(const tracer::vector& p, double dx=0.0001);
	virtual tracer::color ambient(const tracer::shade_context& context);
	virtual tracer::color shade(const tracer::ray& r, const tracer::shade_context& context);
	virtual tracer::color transparency(const tracer::shade_context& context);
	virtual std::ostream& toStream(std::ostream& out) const;
	virtual std::istream& fromStream(std::istream& in);
	virtual tracer::shader* clone();
	double d, Ka, Kd, Kp, phong_size, scale, H, lacunarity,max,bump_size, rust_level;
	tracer::color c1, c2;
	int octaves, type,bump;
};

#endif
