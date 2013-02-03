#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include "tracer.h"
#include "register.h"
#include "utils.h"
#include "camera.h"
#include "raytrace.h"

void saveImage(std::string filename, int width, int height, tracer::color *data) {
  std::cout << "  saving image '" << filename << "' (expose, ";

  //autoExpose(width, height, data);

  std::cout << "save)" << std::endl;
  std::ofstream out(filename.c_str(), std::ios_base::binary);
  tracer::util::writeBmpHeader(width, height, 24, out);
  
  for (int y=0;y<height;y++) {
    for (int x=0;x<width;x++) {
      tracer::util::write8bit(data[y*width + x], out);
    }
  }
}

int main(int argc, char* argv[]) {
  std::string infile = argv[argc-2], outfile = "", filmSize = "35mm";
  double time = 0.0, varianceThreshold = 0.1, max = 0.036;
  int nSamples = 1, nPasses = 1, improvedCameraModel = 0, multiplePasses = 0, width = 640, height = 480;

  registerCommons();

  std::cout << "Tracer v4" << std::endl;
  std::cout.flush();

  std::ifstream in(infile.c_str());
  tracer::camera camera01;
  camera01.fromStream(in);
  tracer::scene *scene01 = tracer::scene_builder::buildScene(in);
  in.close();

  std::cout << camera01 << std::endl;

  for (int i=1;i<argc;i++) {
    std::string arg = argv[i];

    // check flags
    if (arg == "-c") {
      improvedCameraModel = 1;
    } else if (arg == "-h") {
      printHelp();
      exit(0);
    } else if (arg == "-m") {
      multiplePasses = 1;
    } else if (arg[0] == '-') {
      std::string parameter = arg.substr(2, arg.find('=')-2), value = arg.substr(arg.find('=')+1);
      
      if (parameter == "width") {
	width = atoi(value.c_str());
      } else if (parameter == "height") {
	height= atoi(value.c_str());
      } else if (parameter == "flength") {
	camera01.flength = atof(value.c_str());
      } else if (parameter == "fstop") {
	camera01.fstop = atof(value.c_str());
      } else if (parameter == "shutter") {
	camera01.shutterSpeed = atof(value.c_str());
      } else if (parameter == "fdistance") {
	camera01.fdistance = atof(value.c_str());
      } else if (parameter == "filmsize") {
	filmSize = value;	
	if (filmSize == "645" || filmSize == "6x6") {
	  max = 0.060;
	} else if (filmSize == "35mm") {
	  max = 0.036;
	}
      } else if (parameter == "t") {
	time = atof(value.c_str());
      } else if (parameter == "cposition") {
	std::istringstream ss(value);
	ss >> camera01.origin;
      } else if (parameter == "clookat") {
	tracer::vector cl;
	std::istringstream ss(value);
	ss >> cl;
	camera01.lookAt(cl);
	camera01.focusAt(cl);
      } else if (parameter == "nsamples") {
	nSamples = atoi(value.c_str());
      } else if (parameter == "npasses") {
	nPasses = atoi(value.c_str());
      } else if (parameter == "vthreshold") {
	varianceThreshold = atof(value.c_str());
      }       
    } else {
      if (infile == "") {
	infile = arg;
      } else {
	outfile = arg;
      }
    }
  }
  
  
  if (outfile == "") {
    printHelp();
    exit(1);
  }
  
  registerCommons();
  
  double fmaxx = (width < height)? max:max*((double)width/(double)height), fmaxy = (height < width)? max:max*((double)height/(double)width);

  std::cout << "  " << (int)scene01->objects.size() << " object(s)" << std::endl;
  std::cout << "  " << (int)scene01->lightsources.size() << " lightsource(s)" << std::endl << std::endl;

  color *image = new color[width * height], *variance = new color[width * height];
  int *samples = new int[width*height], pixels = 0, rpixels = 0, pointstep = (width*height)/20;
  tracer::color *c = new color[nSamples];

  for (int y=0;y<height;y++) {
    for (int x=0;x<width;x++) {
      image[y*width + x] = variance[y*width + x] = tracer::black;
      samples[y*width + x] = 0;
    }
  }
  
  for (int pass=0;pass<nPasses;pass++) {
    std::cout << "  pass " << (pass+1) << " [                    ]\r  pass " << (pass + 1) << " [";
    std::cout.flush();
    
    pixels = rpixels = 0;

    for (int y=0;y<height;y++) {
      double yf = (((double)y/height) - 0.5)*fmaxy;
      
      for (int x=0;x<width;x++) {
	pixels++;
	double xf = (((double)x/width) - 0.5)*fmaxx;
	int idx = y*width + x;
	
	int i=0;
	
	if (variance[idx].value() > varianceThreshold || pass == 0) {
	  rpixels++;
	  tracer::color sumc = tracer::black, var = tracer::black;
	  for (;i<nSamples;i++) {	  
	    ray r = camera01.createRay(xf, yf, time, improvedCameraModel);
	    c[i] = tracer::raytrace(r, *scene01);
	    sumc += c[i];
	  }
	  
	  for (i=0;i<nSamples;i++) {
	    var += (c[i] - (1.0/nSamples)*sumc).abs();
	  }

	  image[idx] = (1.0/(samples[idx]+nSamples))*(samples[idx]*image[idx] + sumc);
	  variance[idx] = (1.0/(samples[idx]+nSamples))*(samples[idx]*variance[idx] + var);
	  samples[idx] += nSamples;
	}

	variance[idx] = (variance[idx].value() < varianceThreshold)? tracer::black : tracer::white;

	if ((pixels % pointstep) == 0) {
	  std::cout << '.';
	  std::cout.flush();
	}
      }
    }

    std::cout << "] " << rpixels << "/" << pixels << std::endl;
  }

  std::cout << std::endl;
  
  saveImage(outfile + ".bmp", width, height, image);
  saveImage(outfile + "_var.bmp", width, height, variance);
  
  return 0;
}

