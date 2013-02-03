#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include "tracer.h"
#include "register.h"
#include "utils.h"
#include "camera.h"
#include "raytrace.h"

void printHelp() {
  std::cout << "usage: tracer [flags] [options] <infile> <outfile>" << std::endl;
  std::cout << "  flags:" << std::endl;
  std::cout << "    -c                   : use improved camera model" << std::endl;
  std::cout << "    -h                   : prints this message and exits" << std::endl;
  std::cout << "    -m                   : do multiple passes" << std::endl;
  std::cout << std::endl;
  std::cout << "  option:                                                default" << std::endl;
  std::cout << "    --width=<value>      : image width                     640" << std::endl;
  std::cout << "    --height=<value>     : image height                    480" << std::endl;
  std::cout << "    --flength=<value>    : focal length, in millimeters    50mm" << std::endl;
  std::cout << "    --fstop=<value>      : f-stop                          8" << std::endl;
  std::cout << "    --shutter=<value>    : shutter speed is 1/value sec.   125" << std::endl;
  std::cout << "    --fdistance=<value>  : focal distance                  2.0m" << std::endl;
  std::cout << "    --filmsize=<value>   : Size of film (35mm, 645, 6x6)   35mm" << std::endl;
  std::cout << "    --t=<value>          : time, in seconds                0.0" << std::endl;
  std::cout << "    --cposition=<value>  : camera position                 <0,0,2>" << std::endl;
  std::cout << "    --clookat=<value>    : camera \"look at\" point        <0,0,0>" << std::endl;
  std::cout << "    --nsamples=<value>   : number of samples per pixel     1" << std::endl;
  std::cout << "    --npasses=<value>    : number of passes                1" << std::endl;
  std::cout << "    --vthreshold=<value> : variance threshold              0.1" << std::endl;
  std::cout << std::endl;
}

double autoExpose(int width, int height, tracer::color *data) {
  double v, ret = 0.0;

  for (int i=0;i<width*height;i++) {
    v = data[i].value();

    if (v > ret) {
      ret = v;
    }
  }

  for (int i=0;i<width*height;i++) {
    data[i] /= ret;
  }

  return ret;
}

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
  tracer::vector cp(0,0,2), cl(0,0,-1), up(0,1,0), cfront,cright,cup;
  std::string infile = "", outfile = "", filmSize = "35mm";
  double focalLength = 50, fStop = 8, shutterSpeed = 125, focalDistance = 1.0, time = 0.0, varianceThreshold = 0.1, max = 0.036;
  int nSamples = 1, nPasses = 1, improvedCameraModel = 0, multiplePasses = 0, width = 640, height = 480;

  std::cout << "Tracer v4" << std::endl;
  std::cout.flush();

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
      
//      std::cout << parameter << " " << value << std::endl;
      
      if (parameter == "width") {
	width = atoi(value.c_str());
      } else if (parameter == "height") {
	height= atoi(value.c_str());
      } else if (parameter == "flength") {
	focalLength = atof(value.c_str());
      } else if (parameter == "fstop") {
	fStop = atof(value.c_str());
      } else if (parameter == "shutter") {
	shutterSpeed = atof(value.c_str());
      } else if (parameter == "fdistance") {
	focalDistance = atof(value.c_str());
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
	ss >> cp;
      } else if (parameter == "clookat") {
	std::istringstream ss(value);
	ss >> cl;
	focalDistance = (cp - cl).length();
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
  
  cfront = (cl - cp)^1;
  cup = (up - (cfront*up)*cfront)^1;
  cright = cfront.cross(cup)^1;


  //  std::cout << cp << std::endl << cright << std::endl << cup << std::endl << cfront << std::endl << focalDistance << std::endl;

  registerCommons();
  
  std::ifstream in(infile.c_str());
  double fmaxx = (width < height)? max:max*((double)width/(double)height), fmaxy = (height < width)? max:max*((double)height/(double)width);
  tracer::scene *scene01 = tracer::scene_builder::buildScene(in);

  std::cout << "  " << (int)scene01->objects.size() << " object(s)" << std::endl;
  std::cout << "  " << (int)scene01->lightsources.size() << " lightsource(s)" << std::endl << std::endl;

  //tracer::camera camera(cp, tracer::vector(1,0,0), tracer::vector(0,1,0), tracer::vector(0,0,-1));
  tracer::camera camera(cp, cright, cup, cfront);
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
	    ray r = (improvedCameraModel)? camera.createRay(focalLength, fStop, 1.0/shutterSpeed, focalDistance, xf, yf, time):camera.createRay(focalLength, xf, yf, time);
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

