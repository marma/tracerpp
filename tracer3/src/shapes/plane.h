#ifndef __PLANE_H__
#define __PLANE_H__

#include "../tracer.h"
#include <iostream>

class Plane : public tracer::shape {
public:
	Plane(const tracer::vector& _p = tracer::vector(0,0,0), const tracer::vector& _n = tracer::vector(0,1,0));
	virtual void intersect(const tracer::ray& r, tracer::intersection_collection& icoll);
	virtual int inside(const tracer::vector& x, double t);
	virtual shape* clone();
	std::ostream& toStream(std::ostream& out) const;
	std::istream& fromStream(std::istream& in);
	tracer::vector p,n;
};

#endif
