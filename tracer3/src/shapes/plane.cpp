#include "plane.h"

Plane::Plane(const tracer::vector& _p, const tracer::vector& _n) : tracer::shape("Plane"), p(_p), n(_n^1.0) {
}

int Plane::inside(const tracer::vector& x, double t) {
	return (n * ((x - p)^1) > 0 );
}

void Plane::intersect(const tracer::ray& r, tracer::intersection_collection& icoll) {
	// v_scal_n, ray.v scalar n
	double v_scal_n = r.v * n;

	// Test if ray intersects plane at all
	if (v_scal_n<-0.0001) {
			// S1, distance to plane
			double S1 = n * (r.p - p);
			// P2, ray.p projected onto plane
			//tracer::vector P2 = r.p - S1*n;
			// t, distance from ray.p to intersection, I
			double t = S1/v_scal_n;
			// I, intersection
			tracer::vector I(r.p - t*r.v);
			// add I to IntersectionCollection icoll
			icoll.insert(icoll.begin(), tracer::intersection(I, n, -t, r.t, tracer::intersection::ENTER));
	}
}

tracer::shape* Plane::clone() {
	return new Plane(p,n);
}

std::ostream& Plane::toStream(std::ostream& out) const {
	return out << "origo " << p << " normal " << n;
}

std::istream& Plane::fromStream(std::istream& in) {
	std::string tmp = "";

	while (tmp != "}") {
		in >> tmp;

		if (tmp == "origo") {
			in >> p;
		} else if (tmp == "normal") {
			in >> n;
		}
	}

	return in;
}
