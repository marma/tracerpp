#include "tracer_thread.h"

// constructors and destructors
TracerThread::TracerThread(TracerThreadInfo *_tti) : tti(_tti) {    
}

// methods
int TracerThread::run() {
  int line = 0;
  
  while (1) {
    line = tti->getLine();
    
    if (line != -1) {
      int maxx = tti->width, maxy = tti->height;
      double max = tti->max, fmaxx = (maxx < maxy)? max:max*((double)maxx/(double)maxy), fmaxy = (maxy < maxx)? max:max*((double)maxy/(double)maxx); 
      double yf = (((double)line/maxy) - 0.5)*fmaxy;
      
      for (int i=0;i<tti->width;i++) {
	double xf = (((double)i/maxx) - 0.5)*fmaxx;

	tracer::color c = tracer::black;
	for (int sample=0;sample<tti->nSamples;sample++) {
	  tracer::ray r = tti->_camera->createRay(tti->flength, tti->fstop, 1.0/tti->sspeed, tti->fdistance, xf, yf, tti->time);
	  c += tracer::raytrace(r, *tti->_scene);
	}

	tti->set(i,line, (1.0/(double)tti->nSamples)*c);
      }
    } else break;
  }
  
  return 0;
}

