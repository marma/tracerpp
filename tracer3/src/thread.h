#ifndef __THREAD_H__
#define __THREAD_H__

#define _REENTRANT
#include <iostream>
#include <pthread.h>

void* ThreadCreate(void* arg);
class Thread;

struct threadArg {
  Thread *p;
};

class Thread {
public:
enum { STOP, RUN, NOT_INITIALIZED };
// constructors and destructors
	Thread();
	virtual ~Thread();
// methods
	int init();
	virtual int start();
	virtual int run() =0;
	void join();
// operators
// friends
// data members
	pthread_t tid;
	pthread_attr_t attr;
	int state, desiredState;
	threadArg ta;
};

#endif



