#include "aabox.h"

AABox::AABox() : tracer::shape("AABox") {
}

void AABox::intersect(const tracer::ray& r, tracer::intersection_collection& icoll) {
  tracer::vector n1(1,0,0), n2(1,0,0);
  double tmin, tmax, tymin, tymax, tzmin, tzmax;

  if (r.v.x >= 0) {
    tmin = (corner1.x - r.p.x) / r.v.x;
    tmax = (corner2.x - r.p.x) / r.v.x;
  } else {
    tmin = (corner2.x - r.p.x) / r.v.x;
    tmax = (corner1.x - r.p.x) / r.v.x;
  }

  if (r.v.y >= 0) {
    tymin = (corner1.y - r.p.y) / r.v.y;
    tymax = (corner2.y - r.p.y) / r.v.y;
  } else {
    tymin = (corner2.y - r.p.y) / r.v.y;
    tymax = (corner1.y - r.p.y) / r.v.y;
  }

  if ((tmin > tymax) || (tymin > tmax)) {
    return;
  } 

  if (tymin > tmin) {
    tmin = tymin;
    n1 = tracer::vector(0,1,0);
  }

  if (tymax < tmax) {
    tmax = tymax;
    n2 = tracer::vector(0,1,0);
  }

  if (r.v.z >= 0) {
    tzmin = (corner1.z - r.p.z) / r.v.z;
    tzmax = (corner2.z - r.p.z) / r.v.z;
  } else {
    tzmin = (corner2.z - r.p.z) / r.v.z;
    tzmax = (corner1.z - r.p.z) / r.v.z;
  }

  if ((tmin > tzmax) || (tzmin > tmax)) {
    return;
  }

  if (tzmin > tmin) {
    tmin = tzmin;
    n1 = tracer::vector(0,0,1);
  }

  if (tzmax < tmax) {
    tmax = tzmax;
    n2 = tracer::vector(0,0,1);
  }

  if (tmin > 0) {
    if (n1*r.v > 0) n1 *= -1;

    icoll.insert(icoll.end(), tracer::intersection(r(tmin), n1, tmin, r.t, tracer::intersection::ENTER));
  }

  if (tmax > 0) {
    if (n2*r.v < 0) n2 *= -1;
    icoll.insert(icoll.end(), tracer::intersection(r(tmax), n2, tmax, r.t, tracer::intersection::EXIT));
  }
}

int AABox::inside(const tracer::vector& p, double t) {
  return (p.x > corner1.x) && (p.x < corner2.x) && (p.y > corner1.x) && (p.y < corner2.y) && (p.z > corner1.z) && (p.z < corner2.z);
}

std::ostream& AABox::toStream(std::ostream& out) const {
  return out;
}

std::istream& AABox::fromStream(std::istream& in) {
  std::string tmp = "";
  
  while (tmp != "}") {
    in >> tmp;
    
    if (tmp == "corner1") {
      in >> corner1;
    } else if (tmp == "corner2") {
      in >> corner2;
    }
  }
  
  return in;
}

tracer::shape* AABox::clone() {
  return new AABox();
}

