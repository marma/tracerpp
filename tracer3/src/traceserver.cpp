#include <iostream>
#include <string>
#include <sstream>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fstream>
#include "clientsocket.h"
#include "serversocket.h"
#include "socketexception.h"

#define BUF_LEN 10*1204

int maxProcesses;
std::vector<int> children;

void init_count() {
  std::ofstream f(".traceserver.tmp");
  f << 0;
  f.close();
}

int read_count() {
  std::ifstream f(".traceserver.tmp");
  int nActive;

  f >> nActive;
  f.close();

  return nActive;
}

int increase_count() {
  int nActive = read_count();
  
  if (nActive >= maxProcesses) {
    return 0;
  } else {
    std::ofstream f(".traceserver.tmp");
    f << (nActive + 1);
    f.close();
  }

  return 1;
}

int decrease_count() {
  int nActive = read_count();

  if (nActive <= 0) {
    return 0;
  } else {
    std::ofstream f(".traceserver.tmp");
    f << (nActive - 1);
    f.close();
  }

  return 1;

}

void handle_sigchld(int signo) {
  pid_t pid;
  int stat;

  pid = wait(&stat);
  std::cerr << "child " << pid << " terminated!" << std::endl;
}

void handle_sigint(int signo) {
  pid_t pid;
  int stat;

  std::cerr << "closing server socket ... ";
  ssocket.close();
  std::cerr << "done" << std::endl;

  std::
}

std::string read_socket(ClientSocket *cs) {
  char buf[BUF_LEN];
  std::string ret;

  int n = BUF_LEN-1;

  while (n == BUF_LEN-1) {
    n = cs->read(buf, BUF_LEN);

    if (n != 0) {
      buf[n-1] = '\0';
    }
    
    ret += buf;
  }
  
  return ret;
}

std::string purge(std::string in, std::string p) {
  std::string ret;

  for (int i=0;i<in.length();i++) {
    int ok = 1;

    for (int j=0;j<p.length();j++) {
      if (in[i] == p[j]) {
	ok = 0;
	break;
      }
    }

    if (ok) ret += in[i];
  }

  return ret;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "usage: nettracer <port> <max connections>" << std::endl;
    exit(1);
  }

  init_count();
  signal(SIGCHLD, handle_sigchld);

  ServerSocket ssocket(atoi(argv[1]));
  int run = 1;
  maxProcesses = atoi(argv[2]);
  char buf[BUF_LEN];

  while (run) {
    ClientSocket csocket = ssocket.accept();
    std::ostringstream oss;

    oss << "NetTracer v0.01\t" << read_count() <<  "/" << maxProcesses << "\n";

    csocket.write((char*)oss.str().c_str());

    if (purge(read_socket(&csocket), "\r\n") == "quit") {
      run = 0;
      std::cout << "quitting!" << std::endl;
    } else {
      if (!increase_count()) {
	csocket.write("  status: no connections available, sorry\n");
      } else {
	int pid = fork();
	
	// child?
	if (pid == 0) {
	  ssocket.close();
	  int n = csocket.read(buf, BUF_LEN);
	  
	  if (n != 0) {
	    for (int i=n-1;i>=0;i--) {
	      if (buf[i] == '\n' || buf[i] == '\r') {
		buf[i] = '\0';
	      } else {
		break;
	      }
	    }
	    
	    std::string str(buf);
	    
	    std::cout << "got '" << str << "' from client" << std::endl;
	  }
	  
	  decrease_count();
	}
      }
    }
    
    csocket.close();
  }
  
  ssocket.close();

  return 0;
}
