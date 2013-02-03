// tracer.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include "tracer.h"
#include "register.h"
#include "utils.h"
#include "camera.h"
#include "raytrace.h"

int main(int argc, char* argv[]) {
        if (argc != 11) {
            std::cout << "usage: tracer <scene-file> <width> <height> <outfile> <flength> <aperture> <shutter speed> <fdistance> <time> <nsamples>\n";
            exit(1);
        }

	std::string outfilename(argv[4]), bmpfilename = outfilename + ".bmp", xmlfilename = outfilename + ".xml", varfilename = outfilename + "_var.bmp";

	tracer::registerCommons();

	std::ofstream bmpout(bmpfilename.c_str(), std::ios_base::binary), xmlout(xmlfilename.c_str()), varout(varfilename.c_str(), std::ios_base::binary);
	std::ifstream in(argv[1]);

	tracer::scene *scene01 = tracer::scene_builder::buildScene(in);
	tracer::camera camera(tracer::vector(0,0,2), tracer::vector(1,0,0), tracer::vector(0,1,0), tracer::vector(0,0,-1));
	
	in.close();

	std::cout << "\nTracer v3\n";
	std::cout << "  " << (int)scene01->objects.size() << " object(s)\n";
	std::cout << "  " << (int)scene01->lightsources.size() << " lightsource(s)\n";
	std::cout << "\n";
	std::cout << "  tracing [                    ]\r  tracing [";
	std::cout.flush();

	long t1 = clock(), t2;

	int maxx = std::atoi(argv[2]), maxy = std::atoi(argv[3]), pixels=0, pointstep = (maxx*maxy)/20, nSamples = std::atoi(argv[10]);
      	double max = 0.036, fmaxx = (maxx < maxy)? max:max*((double)maxx/(double)maxy), fmaxy = (maxy < maxx)? max:max*((double)maxy/(double)maxx);
	double flength = std::atof(argv[5]), fstop = std::atof(argv[6]), sspeed = std::atof(argv[7]), fdistance = std::atof(argv[8]), time = std::atof(argv[9]);
	tracer::color *c = new color[nSamples];

	tracer::util::writeBmpHeader(maxx,maxy,24,bmpout);
	tracer::util::writeBmpHeader(maxx,maxy,24,varout);

	for (int y=0;y<maxy;y++) {
	  double yf = (((double)y/maxy) - 0.5)*fmaxy;

	  for (int x=0;x<maxx;x++) {
	    pixels++;
	    double xf = (((double)x/maxx) - 0.5)*fmaxx;

	    int i=0;
	    tracer::color sumc = tracer::black, var = tracer::black;
	    for (;i<nSamples;i++) { 
	      ray r = camera.createRay(flength, fstop, 1.0/sspeed, fdistance, xf, yf, time);
	      c[i] = tracer::raytrace(r, *scene01);
	      sumc += c[i];
	    }

   	    sumc *= (1.0/(double)nSamples);

	    for (i=0;i<nSamples;i++) {
	      var += (c[i] - sumc).abs();
	    }

	    var *= (2.0/(double)nSamples);
	    
	    if ((pixels % pointstep) == 0) {
	      std::cout << '.';
	      std::cout.flush();
	    }
	    
	    tracer::util::write8bit(sumc, bmpout);
	    tracer::util::write8bit(var, varout);
	  }
	}

	bmpout.close();
	varout.close();

	xmlout << "<?xml version=\"1.0\" encoding=\"iso-8859-1\">" << std::endl;
	xmlout << "<info>" << std::endl;
	xmlout << "  <camera>" << std::endl;
	xmlout << "    <focal_length>" << flength << "</focal_length>" << std::endl;
	xmlout << "    <fstop>" << fstop << "</fstop>" << std::endl;
	xmlout << "    <shutter_speed>" << sspeed << "</shutter_speed>" << std::endl;
	xmlout << "    <focal_distance>" << fdistance << "</focal_distance>" << std::endl;
	xmlout << "    <position x=\"" << camera.origin.x << "\" y=\"" << camera.origin.y << "\" z=\"" << camera.origin.z << "\"/>" << std::endl;
	xmlout << "    <direction x=\"" << camera.front.x << "\" y=\"" << camera.front.y << "\" z=\"" << camera.front.z << "\"/>" << std::endl;
	xmlout << "  </camera>" << std::endl;
	xmlout << "</info>" << std::endl;

	xmlout.close();
	
	in.close();

	std::cout << "\n\n";
	
	t2 = clock();

	std::cout << "  CPU time: " << (t2 - t1)/1000 << " milliseconds\n";

	return 0;
}

