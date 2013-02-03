#ifndef __SERVERSOCKET_H__
#define __SERVERSOCKET_H__

#include "clientsocket.h"

class ServerSocket {
public:
	ServerSocket(u_short _port);
	ClientSocket accept(int timeoutMillis = 0);
	void flush();
	void close();
	~ServerSocket();
private:
	SOCKET serversocket;
	u_short port;
	sockaddr_in name;
	socklen_t len;
	int state;
};

#endif
