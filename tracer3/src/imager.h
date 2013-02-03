#ifndef __IMAGER_H__
#define __IMAGER_H__

#include "serializable.h"
#include "color.h"

namespace tracer {
  class imager : public serializable {
  public:
    struct bits8x3 {
      bits8x3() : r(0), g(0), b(0) { }
      bits8x3(unsigned char _r, unsigned char _g, unsigned char _b) : r(_r), g(_g), b(_b) {}
      unsigned char r,g,b;
    };

    struct bits16x3 {
      bits16x3(unsigned short _r, unsigned short _g, unsigned short _b) : r(_r), g(_g), b(_b) {}
      unsigned short r,g,b;
    };

    imager() : serializable("imager"), inlevels_min(0), inlevels_max(1.0), levels_pow(1.0), outlevels_min(0), outlevels_max(1), dither(0.05), nBits(8) {
    }

    color process(tracer::color c) {
      //color n = (tracer::white - c.exp(-2)).map(0, 1, inlevels_min, inlevels_max).pow(levels_pow) + (rand()%2000 - 1000)/1000.0*dither*tracer::white.clamp(0,1); 

      color n = (tracer::white - c.exp(-2)).map(outlevels_min, outlevels_max, inlevels_min, inlevels_max).pow(levels_pow).clamp(0,1);

      return n;
    }

    bits8x3 to8bits(tracer::color c) {
      unsigned char bits[3];

      //      bits[0] = (unsigned char)(255*(1.0 - exp(-2*c.r)));
      //      bits[1] = (unsigned char)(255*(1.0 - exp(-2*c.g)));
      //      bits[2] = (unsigned char)(255*(1.0 - exp(-2*c.b)));
      bits[0] = (unsigned char)(255*c.r);
      bits[1] = (unsigned char)(255*c.g);
      bits[2] = (unsigned char)(255*c.b);

      // work around silly newline stuff
      if (bits[0] == '\n') bits[0] += 1;
      if (bits[1] == '\n') bits[1] += 1;
      if (bits[2] == '\n') bits[2] += 1;

      return bits8x3(bits[0], bits[1], bits[2]);
    }

    bits16x3 to16bits(tracer::color c) {
      unsigned short bits[3];

      return bits16x3(bits[0], bits[1], bits[2]);
    }

    void write(tracer::color c, std::ostream& out) {
      if (nBits == 8) {
	bits8x3 b = to8bits(c);

        out.put(b.r);
        out.put(b.b);
	out.put(b.g);
      } else if (nBits == 16) {
      }
    }
   
    virtual std::istream& fromStream(std::istream& in) {
      std::string token;

      in >> token;

      while (token != "}") {
	if (token == "inlevels_min" ) {
	  in >> inlevels_min;
        } else if (token == "inlevels_max") {
	  in >> inlevels_max;
        } else if (token == "outlevels_min") {
	  in >> outlevels_min;
	} else if (token == "outlevels_max") {
	  in >> outlevels_max;
        } else if (token == "levels_pow") {
	  in >> levels_pow;
        } else if (token == "dither") {
	  in >> dither;
	} else if (token == "bits") {
	  in >> nBits;
	}

	in >> token;
      }    

      return in;
    }

    virtual std::ostream& toStream(std::ostream& out) const {
      return out << "inlevels_min " << inlevels_min << " inlevels_max " << inlevels_max << " levels_pow" << levels_pow << " outlevels_min " << outlevels_min << " outlevels_max " << outlevels_max << " dither " << dither << " bits" << nBits << " ";
    }

    virtual imager *clone() {
      return new imager;
    }

  protected:
    double inlevels_min, inlevels_max, levels_pow, outlevels_min, outlevels_max, dither;
    int nBits;
  };
}

#endif
