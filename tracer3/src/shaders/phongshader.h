#ifndef __PHONGSHADER_H__
#define __PHONGSHADER_H__

#include "../shader.h"
#include "../raytrace.h"

class PhongShader : public tracer::shader {
public:
	PhongShader(double _Ka = 0.0, double _Kd = 0.8, double _Kp = 0.6, double _phong_size = 20, const tracer::color& _c = tracer::blue);
	virtual tracer::color shade(const tracer::ray& r, const tracer::shade_context& context);
	virtual tracer::color ambient(const tracer::shade_context& context);
	virtual std::ostream& toStream(std::ostream& out) const;
	virtual std::istream& fromStream(std::istream& in);
	virtual tracer::shader* clone();
	double Ka, Kd, Kp, phong_size;
	tracer::color c;
	int nAmbientSamples;
};

#endif
