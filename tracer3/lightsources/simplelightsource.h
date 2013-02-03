#ifndef __SIMPLELIGHTSOURCE_H__
#define __SIMPLELIGHTSOURCE_H__

#include "../tracer.h"

class SimpleLightsource : public tracer::lightsource {
	public:
		SimpleLightsource(const tracer::vector& _p = tracer::vector(0,0,0), const tracer::color _c = tracer::color(1,1,1)) : lightsource("SimpleLightsource"), p(_p), c(_c) {
		}

		virtual tracer::ray shade(const tracer::vector& P, double t) {
			//if ((P-p).length() < 0.5) return ray(p, (P - p)^1, c);
			//else return ray(p, (P - p)^1, 0*c);

			return tracer::ray(p, (P - p)^1, t, 1.0/((P-p).length2()+1)*c);
		}

		virtual int inside(const tracer::vector& p, double t) {
			return 1;
		}

		virtual std::ostream& toStream(std::ostream& out) const {
			out << " position " << p << " color " << c;

			return out;
		}

		virtual std::istream& fromStream(std::istream& in) {
			std::string tmp = "";

			while (tmp != "}") {
				in >> tmp;

				//std::cout << "tmp is " << tmp << std::endl;

				if (tmp == "position") {
					in >> p;
				} else if (tmp == "color") {
					in >> c;
				}
			}

			return in;
		}

		virtual tracer::lightsource* clone() {
			return new SimpleLightsource(p, c);
		}

		tracer::vector p;
		tracer::color c;
		static int registered;
};

#endif

