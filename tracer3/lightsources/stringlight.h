#ifndef __STRINGLIGHTSOURCE_H__
#define __STRINGLIGHTSOURCE_H__

#include "../tracer.h"

class StringLightsource : public tracer::lightsource {
	public:
		StringLightsource(const tracer::vector& _p = tracer::vector(0,0,0), tracer::vector _v = tracer::vector(0,1,0), const tracer::color _c = tracer::color(1,1,1)) : lightsource("StringLightsource"), p(_p), v(_v), c(_c) {
		}

		virtual tracer::ray shade(const tracer::vector& P, double t) {
			double r = (rand()%1000)/1000.0;
			tracer::vector p2(p + r*v);

			return tracer::ray(p2, (P - p2)^1, t, 1.0/((P-p2).length2()+1)*c);
		}

		virtual int inside(const tracer::vector& p, double t) {
			return 1;
		}

		virtual std::ostream& toStream(std::ostream& out) const {
			out << "position " << p << " v " << v << " color " << c;

			return out;
		}

		virtual std::istream& fromStream(std::istream& in) {
			std::string tmp = "";

			while (tmp != "}") {
				in >> tmp;

				if (tmp == "position") {
					in >> p;
				} else if (tmp == "v") {
					in >> v;
				} else if (tmp == "position") {
					in >> c;
				}
			}

			return in;
		}

		virtual tracer::lightsource* clone() {
			return new StringLightsource(p, v, c);
		}

		tracer::vector p,v;
		tracer::color c;
};


#endif

