#ifndef __CLIENTSOCKET_H__
#define __CLIENTSOCKET_H__


#include <iostream>

#ifdef __unix
#define SOCKET int
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#endif

#ifdef __WINDOWS__
#include <winsock2.h>
#endif

#include <errno.h>
#include "socketexception.h"

enum { CONNECTED, NOT_CONNECTED, SHUTDOWN, ERROR };

class ClientSocket {
public:
        ClientSocket();
	ClientSocket(const ClientSocket& c);
	ClientSocket(SOCKET _clientsocket);
	ClientSocket(char* _hostname, u_short _port);
	~ClientSocket();
	int read(char* buffer, int length, int flags = 0);
        int write(char* buffer);
	int write(char* buffer, int length, int flags = 0);
	int connect();
	void flush();
	void close();
	void shutdown(int howto);
	ClientSocket& operator = (ClientSocket& cs);
private:
	SOCKET clientsocket;
	char hostname[64]; 
	u_short port;
	struct sockaddr_in name;
	int len;
	struct hostent *hp;
	int state;
};

#endif

