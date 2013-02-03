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
#include "tracer_thread_info.h"
#include "tracer_thread.h"


typedef Thread* threadp;

int main(int argc, char* argv[]) {
        if (argc != 12) {
            std::cout << "usage: tracer <scene-file> <width> <height> <outfile> <flength> <fstop> <shutter speed> <fdistance> <time> <nsamples> <nthreads>\n";
            exit(1);
        }

	std::string outfilename(argv[4]), bmpfilename = outfilename + ".bmp";

	tracer::registerCommons();

	std::ifstream in(argv[1]);

	tracer::scene *scene01 = tracer::scene_builder::buildScene(in);
	tracer::camera camera(tracer::vector(0,0,2), tracer::vector(1,0,0), tracer::vector(0,1,0), tracer::vector(0,0,-1));
	
	in.close();

	std::cout << "\nTracer v3\n";
	std::cout << "  " << (int)scene01->objects.size() << " object(s)\n";
	std::cout << "  " << (int)scene01->lightsources.size() << " lightsource(s)\n";
	std::cout.flush();

	int maxx = std::atoi(argv[2]), maxy = std::atoi(argv[3]), nSamples = std::atoi(argv[10]), nThreads = std::atoi(argv[11]);
      	double time = std::atof(argv[9]);
	double flength = std::atof(argv[5]), fstop = std::atof(argv[6]), sspeed = std::atof(argv[7]), fdistance = std::atof(argv[8]);

	TracerThreadInfo tti(scene01, &camera, maxx, maxy, 0.036, flength, fstop, sspeed, fdistance, time, nSamples);
	threadp *threads = new threadp[nThreads];

	for (int i=0;i<nThreads;i++) {
	  threads[i] = new TracerThread(&tti);
	  threads[i]->start();
	}
	
	for (int i=0;i<nThreads;i++) {
	  threads[i]->join();
	}

	tti.save(bmpfilename);

	return 0;
}

