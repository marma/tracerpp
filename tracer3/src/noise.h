#ifndef __NOISE_H__
#define __NOISE_H__

#include "vector.h"

namespace tracer {
  namespace noise{
    namespace perlin {
      static unsigned int P[] = {
	151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,
	69,142,8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,
	252,219,203,117, 35,11, 32,57,177,33,88,237,149,56,87,174,20,125,136,
	171,168, 68,175, 74,165,71,134,139, 48,27,166, 77,146,158,231,83,111,
	229,122,60,211,133,230,220,105, 92,41,55, 46,245, 40,244,102,143, 54,
	65,25,63,161, 1,216, 80,73,209,76,132,187,208, 89,18,169,200,196,135,
	130,116,188,159, 86,164,100,109,198,173,186, 3,64,52,217,226,250,124,
	123, 5,202, 38,147,118,126,255,82, 85,212,207,206,59,227,47,16,58,17,
	182,189,28, 42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,
	101,155,167,43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,
	185,112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,
	241, 81,51,145,235,249,14,239,107,49,192,214, 31,181,199,106,157,184,
	84,204,176,115,121,50,45,127, 4,150,254,138,236,205,93,222,114,67,29,
	24,72,243,141,128,195,78,66,215,61,156,180,

	151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,
	69,142,8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,
	252,219,203,117, 35,11, 32,57,177,33,88,237,149,56,87,174,20,125,136,
	171,168, 68,175, 74,165,71,134,139, 48,27,166, 77,146,158,231,83,111,
	229,122,60,211,133,230,220,105, 92,41,55, 46,245, 40,244,102,143, 54,
	65,25,63,161, 1,216, 80,73,209,76,132,187,208, 89,18,169,200,196,135,
	130,116,188,159, 86,164,100,109,198,173,186, 3,64,52,217,226,250,124,
	123, 5,202, 38,147,118,126,255,82, 85,212,207,206,59,227,47,16,58,17,
	182,189,28, 42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,
	101,155,167,43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,
	185,112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,
	241, 81,51,145,235,249,14,239,107,49,192,214, 31,181,199,106,157,184,
	84,204,176,115,121,50,45,127, 4,150,254,138,236,205,93,222,114,67,29,
	24,72,243,141,128,195,78,66,215,61,156,180
      };

      inline double fade(double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
      }

      inline double lerp(double t, double a, double b) { 
	return a + t * (b - a); 
      }

      inline double grad(int hash, double x, double y, double z) {
	int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
	double u = h<8||h==12||h==13 ? x : y,   // INTO 12 GRADIENT DIRECTIONS.
	  v = h<4||h==12||h==13 ? y : z;

	return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
      }

      inline double noise(const tracer::vector& p) {
	double x = p.x, y = p.y, z = p.z;
	int X = (int)::floor(p.x) & 0xFF,
	    Y = (int)::floor(p.y) & 0xFF,
	    Z = (int)::floor(p.z) & 0xFF;

	x -= ::floor(x);
	y -= ::floor(y);
	z -= ::floor(z);

	double u = fade(x), v = fade(y), w = fade(z);
	int A = P[X  ]+Y, AA = P[A]+Z, AB = P[A+1]+Z,  // HASH COORDINATES OF
            B = P[X+1]+Y, BA = P[B]+Z, BB = P[B+1]+Z;  // THE 8 CUBE CORNERS,

	return lerp(w, lerp(v, lerp(u, grad(P[AA  ], x  , y  , z   ),  // AND ADD
				       grad(P[BA  ], x-1, y  , z   )), // BLENDED
			       lerp(u, grad(P[AB  ], x  , y-1, z   ),  // RESULTS
				       grad(P[BB  ], x-1, y-1, z   ))),// FROM  8
		       lerp(v, lerp(u, grad(P[AA+1], x  , y  , z-1 ),  // CORNERS
				       grad(P[BA+1], x-1, y  , z-1 )), // OF CUBE
			       lerp(u, grad(P[AB+1], x  , y-1, z-1 ),
				       grad(P[BB+1], x-1, y-1, z-1 ))));
      }

      inline double fBm(const tracer::vector& point, double H, double lacunarity, double octaves) {
	tracer::vector p = point;
	double value = 0, l = 1;

	for (int i=0;i<octaves;i++) {
	  value += tracer::noise::perlin::noise(p) * pow(l, -H);
	  l *= lacunarity;
	  p *= lacunarity;
	}

	return value;
      }

      inline double tracer::noise::perlin::VLnoise(const tracer::vector& p, double d) {
	tracer::vector offset = tracer::vector(tracer::noise::perlin::noise(p + tracer::vector(0.5,0,0)), tracer::noise::perlin::noise(p + tracer::vector(0,0.5,0)), tracer::noise::perlin::noise(p + tracer::vector(0,0,0.5)));

	return tracer::noise::perlin::noise(p + d*offset);
      }

      inline double VLfBm(const tracer::vector& point, double d, double H, double lacunarity, double octaves) {
	tracer::vector p = point;	
	double value = 0, l = 1;

	for (int i=0;i<octaves;i++) {
	  value += tracer::noise::perlin::VLnoise(p,d) * pow(l, -H);
	  l *= lacunarity;
	  p *= lacunarity;
	}

	return value;
      }

      inline tracer::vector dnoise(const tracer::vector& p, double dx=0.0001) {
	double n = tracer::noise::perlin::noise(p);
	return tracer::vector((tracer::noise::perlin::noise(tracer::vector(p.x + dx, p.y, p.z)) - n)/dx,
			      (tracer::noise::perlin::noise(tracer::vector(p.x, p.y + dx, p.z)) - n)/dx,
			      (tracer::noise::perlin::noise(tracer::vector(p.x, p.y, p.z + dx)) - n)/dx);
      }

      inline tracer::vector dfBm(const tracer::vector& p, double H, double lacunarity, double octaves, double dx=0.001) {
	double n = tracer::noise::perlin::fBm(p, H, lacunarity, octaves);
	return tracer::vector((tracer::noise::perlin::fBm(tracer::vector(p.x + dx, p.y, p.z), H, lacunarity, octaves) - n)/dx,
			      (tracer::noise::perlin::fBm(tracer::vector(p.x, p.y + dx, p.z), H, lacunarity, octaves) - n)/dx,
			      (tracer::noise::perlin::fBm(tracer::vector(p.x, p.y, p.z + dx), H, lacunarity, octaves) - n)/dx);
      }

      inline tracer::vector dVLnoise(const tracer::vector& p, double d, double dx=0.0001) {
	double n = tracer::noise::perlin::VLnoise(p,dx);
	return tracer::vector((tracer::noise::perlin::VLnoise(tracer::vector(p.x + dx, p.y, p.z), d) - n)/dx,
			      (tracer::noise::perlin::VLnoise(tracer::vector(p.x, p.y + dx, p.z), d) - n)/dx,
			      (tracer::noise::perlin::VLnoise(tracer::vector(p.x, p.y, p.z + dx), d) - n)/dx);
      }

      inline tracer::vector dVLfBm(const tracer::vector& p, double d, double H, double lacunarity, double octaves, double dx=0.001) {
	double n = tracer::noise::perlin::VLfBm(p, d, H, lacunarity, octaves);
	return tracer::vector((tracer::noise::perlin::VLfBm(tracer::vector(p.x + dx, p.y, p.z), d, H, lacunarity, octaves) - n)/dx,
			      (tracer::noise::perlin::VLfBm(tracer::vector(p.x, p.y + dx, p.z), d, H, lacunarity, octaves) - n)/dx,
			      (tracer::noise::perlin::VLfBm(tracer::vector(p.x, p.y, p.z + dx), d, H, lacunarity, octaves) - n)/dx);
      }
    }
  }
}

#endif
