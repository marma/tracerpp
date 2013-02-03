#ifndef __SIMPLEAREALIGHTSOURCE_H__
#define __SIMPLEAREALIGHTSOURCE_H__

#include "../tracer.h"

class SimpleAreaLightsource : public tracer::lightsource {
	public:
		SimpleAreaLightsource(const tracer::vector& _p = tracer::vector(0,0,0), double _r = 1.0, const tracer::color _c = tracer::color(1,1,1)) : lightsource("SimpleAreaLightsource"), p(_p), c(_c), r(_r) {
		}

		virtual tracer::ray shade(const tracer::vector& P, double t) {
			tracer::vector dx((rand()%2000-1000)/1000.0, (rand()%2000-1000)/1000.0, (rand()%2000-1000)/1000.0);

			while (dx.length2() > 1.0) dx = tracer::vector((rand()%2000-1000)/1000.0, (rand()%2000-100)/1000.0, (rand()%2000-1000)/1000.0);

			tracer::vector p2(p + r*dx);

			return tracer::ray(p2, (P - p2)^1, t, 1.0/((P-p2).length2()+1)*c);
		}

		virtual int inside(const tracer::vector& p, double t) {
			return 1;
		}

		virtual std::ostream& toStream(std::ostream& out) const {
			out << "position " << p << " radius " << r << " color " << c;

			return out;
		}

		virtual std::istream& fromStream(std::istream& in) {
			std::string tmp = "";

			while (tmp != "}") {
				in >> tmp;

				if (tmp == "position") {
					in >> p;
				} else if (tmp == "radius") {
					in >> r;
				} else if (tmp == "color") {
					in >> c;
				}
			}

			return in;
		}

		virtual tracer::lightsource* clone() {
			return new SimpleAreaLightsource(p, r, c);
		}

		tracer::vector p;
		tracer::color c;
		double r;
};

#endif

