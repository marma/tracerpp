#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "clientsocket.h"
#include "serversocket.h"
#include "socketexception.h"
#include "tracer.h"
#include "register.h"
#include "utils.h"
#include "camera.h"
#include "raytrace.h"
#include "imager.h"

#define BUF_LEN 10*1204

unsigned int maxProcesses;
std::vector<int> children;
ServerSocket *ss;


void register_handler(int signo, void (*disp)(int)) {
  struct sigaction act, oact;

  act.sa_handler = disp;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  #ifdef SA_INTERRUPT
  act.sa_flags |= SA_INTERRUPT;
  #endif

  #ifdef SA_RESTART
  act.sa_flags |= SA_RESTART;
  #endif

  if (sigaction(signo, &act, &oact) < 0) {
    //return SIG_ERR;
  } else {
    //return oact.sa_handler;
  }
}

void handle_sigchld(int signo) {
  pid_t pid;
  int stat;

  pid = wait(&stat);
  std::cerr << "child " << pid << " terminated!" << std::endl;

  for (std::vector<int>::iterator iter = children.begin(); iter != children.end(); iter++) {
    if (*iter == pid) {
      children.erase(iter);
      break;
    }
  }
}


std::string readline_socket(ClientSocket *cs) {
  char c = ' ';
  std::string ret;

  while (c != '\n') {
    int n = cs->read(&c, 1);

    if (n != 0 && c != '\r' && c != '\n') {
      ret += c;
    }
  }

  //std::cerr << getpid() << ": readline returned '" << ret << "'" << std::endl << std::flush;

  return ret;
}


std::string purge(std::string in, std::string p) {
  std::string ret;

  for (unsigned int i=0;i<in.length();i++) {
    int ok = 1;

    for (unsigned int j=0;j<p.length();j++) {
      if (in[i] == p[j]) {
	ok = 0;
	break;
      }
    }

    if (ok) ret += in[i];
  }

  return ret;
}


void quit() {
  std::cerr << getpid() << ": closing server socket" << std::endl << std::flush;
  ss->close();

  if (!children.empty()) {
    std::cout << getpid() << ": waiting for children" << std::endl << std::flush;
    
    for (int i=0;i<6;i++) {
      sleep(2);
      if (children.empty()) break;
    }
    
    if (!children.empty()) {
      for (unsigned int i=0;i<children.size();i++) {
	kill(children[i], SIGINT);
      }
    }
    
    std::cout << getpid() <<": waiting for children done" << std::endl << std::flush;
  }

  exit(0);
}


void handle_sigint(int signo) {
  quit();
}

void render_client(ClientSocket *cs) {
  // read scene from
  int len = atoi(readline_socket(cs).c_str());
  char *buf = (char*)malloc(len+1);

  int n = -1, sum = 0;

  while (sum != len && n != 0) {
    n = cs->read(&(buf[sum]), len);
    sum += n;
  }

  buf[len] = '\0';

  //std::cerr << "scene is " << len << " bytes, received " << sum << " bytes from client" << std::endl << std::flush;
  
  cs->write((char*)"OK\n");

  std::string str(buf);
  std::istringstream in(str);

  tracer::camera camera01;
  camera01.fromStream(in);
  tracer::imager imager01;
  imager01.fromStream(in);
  tracer::scene *scene01 = tracer::scene_builder::buildScene(in);

  // render loop
  std::string cmd = readline_socket(cs);
  while (cmd != "QUIT") {
    if (cmd == "RENDER AREA" || cmd == "PREVIEW AREA") {
      std::istringstream in(readline_socket(cs));
      int width, height, startX, startY, stopX, stopY, improvedCameraModel, nSamples;
      double time;
      
      in >> width >> height >> startX >> startY >> stopX >> stopY >> improvedCameraModel >> time >> nSamples;
      unsigned char *data = new unsigned char[4*(stopX-startX+1)];


      double fmaxx = ((double)width/(double)height)*0.024, fmaxy = 0.024;
      for (int y=startY; y<=stopY; y++) {
	double yf = (((double)(height-y)/height) - 0.5)*fmaxy;
	for (int x=startX; x<=stopX; x++) {
	  double xf = (((double)x/width) - 0.5)*fmaxx;

	  unsigned int r = 0, g = 0, b = 0;
	  for (int i=0;i<nSamples;i++) {
	    tracer::color c;
	    tracer::imager::bits8x3 bits;

	    if (cmd == "PREVIEW AREA") {
	      ray rayoflight = camera01.createRay(xf, yf, time, 0);
	      c = tracer::preview(rayoflight, *scene01);
	      bits = imager01.to8bits(c);
	    } else {
	      ray rayoflight = camera01.createRay(xf, yf, time, improvedCameraModel);
	      c = tracer::raytrace(rayoflight, *scene01);
	      bits = imager01.to8bits(imager01.process(c));
	    }

	    r += (unsigned int)bits.r;
	    g += (unsigned int)bits.g;
	    b += (unsigned int)bits.b;
	  }
	  
	  data[4*(x - startX)] = '\255';
	  data[4*(x - startX)+1] = r/nSamples;
	  data[4*(x - startX)+2] = g/nSamples;
	  data[4*(x - startX)+3] = b/nSamples;
	}
	
	cs->write((char*)data, 4*(stopX-startX+1));
      }
      
      delete[] data;
    }

    cmd = readline_socket(cs);
  }

  exit(0);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "usage: nettracer <port> <max connections>" << std::endl;
    exit(1);
  }

  registerCommons();

  // custom signal handlers
  signal(SIGCHLD, handle_sigchld);
  signal(SIGINT, handle_sigint);

  ServerSocket ssocket(atoi(argv[1]));
  ss = &ssocket;
  int run = 1;
  maxProcesses = atoi(argv[2]);

  while (run) {
    ClientSocket csocket;

    try {
      ClientSocket s = ssocket.accept();
      csocket = s;
    } catch (SocketException e) {
      std::cerr << getpid() << ": " << e.getMessage() << std::endl << std::flush;
      if (e.getErrno() == EINTR) continue;
      quit();
    }
    
    std::ostringstream oss;
    oss << "NetTracer v0.01\t" << children.size() <<  "/" << maxProcesses << "\n";
    std::string cmd;
    
    // send init string
    csocket.write((char*)oss.str().c_str());
    
    // read command
    cmd = readline_socket(&csocket);
    
    if (cmd == "QUIT") {
      quit();
    } else if (cmd == "RESTART") {
      
    } else if (cmd == "BYE") {
      csocket.close();
    } else if (cmd == "HELO") {
      if (children.size() >= maxProcesses) {
	csocket.write("BUSY\n");
      } else {
	int pid = fork();
	
	// child?
	if (pid == 0) {
	  // restore default SIGINT handler (terminate)
	  signal(SIGINT, SIG_DFL);
	  

	  // set priority
	  setpriority(PRIO_PROCESS, getpid(), -20);

	  // close server socket
	  //ssocket.close();
	  
	  // send OK
	  csocket.write("OK\n");
	  
	  // do stuff
	  
	  try {
	    render_client(&csocket);
	  } catch (SocketException e) {
	    std::cerr << getpid() << ": " << e.getMessage() << std::endl << std::flush;
	  }

	  csocket.close();

	  exit(0);
	} else {
	  children.push_back(pid);
	  csocket.close();
	}
      }
    }
  }

  return 0;
}
