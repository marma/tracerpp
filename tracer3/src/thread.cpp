#include "thread.h"

Thread::Thread() {
	state = NOT_INITIALIZED;
	ta.p = this;
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
}

Thread::~Thread() {
	state = NOT_INITIALIZED;
}

int Thread::init() {
	return 0;
}

void Thread::join() {
  pthread_join(tid, NULL);
}

int Thread::start() {
	int retcode = -1;

	if (state == NOT_INITIALIZED) {
		//std::cout << "Starting thread\n";
		//std::cout.flush();
      		retcode = pthread_create(&tid, &attr, ThreadCreate, (void*)&ta);
	}

	return retcode;
}

void* ThreadCreate(void* arg) {
	threadArg *ta = (threadArg*)arg;

	//pthread_detach(ta->p->tid);

	ta->p->init();
	ta->p->state = Thread::RUN;
	ta->p->run();
	ta->p->state = Thread::STOP;

	//delete ta->p;

	return 0;
}






