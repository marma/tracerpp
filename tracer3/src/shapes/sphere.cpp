#include "sphere.h"

Sphere::Sphere(const tracer::vector& _p, double _radius) : tracer::shape("Sphere"), p(_p), radius(_radius) {
  r2 = radius*radius;
}

int Sphere::inside(const tracer::vector& x, double t) {
	return (p - x).length2() <= r2;
}

void Sphere::intersect(const tracer::ray& r, tracer::intersection_collection& icoll) {
  tracer::vector pp = p;//.rotate(tracer::vector(0,1,0), 2*3.14159*r.t);

  if (!position_spline.empty()) {
    pp = position_spline(r.t);
  }

  double X0 = r.p.x, Y0 = r.p.y, Z0 = r.p.z, Xd = r.v.x, Yd = r.v.y, Zd = r.v.z, Xc = pp.x, Yc = pp.y, Zc = pp.z,
    B = 2 * (Xd * (X0 - Xc) + Yd * (Y0 - Yc) + Zd * (Z0 - Zc)),
    C = (X0 - Xc)*(X0 - Xc) + (Y0 - Yc)*(Y0 - Yc) + (Z0 - Zc)*(Z0 - Zc) - r2,
    disc = B*B - 4*C;

  if (disc<0) {
    return;
  } else {
    double t0 = (-B - sqrt(disc))/2, t1 = (-B + sqrt(disc))/2;
    if (t0>t1) { double tmp=t0;t0=t1;t1=tmp; }

    if (t0>0) icoll.insert(icoll.end(), tracer::intersection(r(t0), (r(t0)-pp)^1, t0, r.t, tracer::intersection::ENTER));
    if (t1>0) icoll.insert(icoll.end(), tracer::intersection(r(t1), (r(t1)-pp)^1, t1, r.t, tracer::intersection::EXIT));
  }
}

tracer::shape* Sphere::clone() {
	return new Sphere(p,radius);
}

std::ostream& Sphere::toStream(std::ostream& out) const {
	return out << "position " << p << " radius " << radius;
}

std::istream& Sphere::fromStream(std::istream& in) {
	std::string tmp = "";

	while (tmp != "}") {
		in >> tmp;

		if (tmp == "position") {
			in >> p;
		} else if (tmp == "radius") {
			in >> radius;
			r2 = radius*radius;
		} else if (tmp == "position_spline") {
		  in >> position_spline;
		}
	}

	return in;
}


