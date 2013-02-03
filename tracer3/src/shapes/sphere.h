#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "../tracer.h"
#include "../math/linear_spline.h"

using namespace tracer;

class Sphere : public shape {
public:
	Sphere(const tracer::vector& _p = tracer::vector(0,0,0), double _radius = 1.0);
	virtual void intersect(const ray& r, intersection_collection& icoll);
	virtual int inside(const tracer::vector& x, double t);
	virtual std::ostream& toStream(std::ostream& out) const;
	virtual std::istream& fromStream(std::istream& in);
	virtual shape* clone();
	linear_spline<tracer::vector> position_spline;
	tracer::vector p;
	double radius,r2;
};

#endif

