#include "phongshader.h"

PhongShader::PhongShader(double _Ka, double _Kd, double _Kp, double _phong_size, const tracer::color& _c) : tracer::shader("PhongShader"), Ka(_Ka), Kd(_Kd), Kp(_Kp), phong_size(_phong_size), c(_c), nAmbientSamples(8) {
}

tracer::color PhongShader::shade(const tracer::ray& r, const tracer::shade_context& context) {
	tracer::color diffuse = tracer::black, specular = tracer::black;
	double d = -context.N * r.v;

	d = fabs(d);

	if (d>0) { 
		// diffuse
		diffuse = Kd*d*r.c*c;
		//diffuse = tracer::color(fabs(context.u), fabs(context.v), fabs(1 - (context.v + context.u)));
		//diffuse = tracer::color(fabs(context.N.x), fabs(context.N.y), fabs(context.N.z));

		// specular
		tracer::vector N = context.N;
		tracer::vector L = r.v;
		tracer::vector V = context.E;
		tracer::vector R = (2*(N * L)*N - L)^1.0;

		specular = Kp*pow(R * V, phong_size)*r.c;
	}
	
	return diffuse + specular;
}

tracer::color PhongShader::ambient(const tracer::shade_context& context) {
  //return tracer::color((context.world_N.x+0.5)/2.0, (context.world_N.y+0.5)/2.0, (context.world_N.z+0.5)/2.0);
  return (Ka == 0.0)? tracer::black:Ka*c*tracer::ambient(context.world_P, context.world_N, *context.world, PI, nAmbientSamples);
}

std::ostream& PhongShader::toStream(std::ostream& out) const {
	return out << "Ka " << Ka << " Kd " << Kd << " Kp " << Kp << " phong_size " << phong_size << " color " << c;
}

std::istream& PhongShader::fromStream(std::istream& in) {
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
		} else if (tmp == "color") {
			in >> c;
		} else if (tmp == "ambient_samples") {
		  in >> nAmbientSamples;
		}
	}

	return in;
}

tracer::shader* PhongShader::clone() {
	return new PhongShader(Ka, Kd, Kp, phong_size, c);
}
