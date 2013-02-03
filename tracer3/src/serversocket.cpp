#include "serversocket.h"

ServerSocket::ServerSocket(u_short _port) : port(_port) {
  int n;
  if ((serversocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) throw SocketCreationException(errno);

  memset(&name, 0, sizeof(struct sockaddr_in));

  name.sin_family = AF_INET;
  name.sin_port = htons(port);
  len = sizeof(struct sockaddr_in);

  n = INADDR_ANY;
  memcpy(&name.sin_addr, &n, sizeof(long));

  if (bind(serversocket, (struct sockaddr *) &name, len) < 0) throw BindException(errno);
  
  if (listen(serversocket,5)) throw ListenException(errno);
}

ServerSocket::~ServerSocket() {
  if (state != NOT_CONNECTED) {
    //close();
  }
}

ClientSocket ServerSocket::accept(int timeoutMillis) {
  SOCKET cs;

  std::cerr << getpid() << ": accept()" << std::endl;
  std::cerr.flush();
  
  if ((cs = ::accept(serversocket, (struct sockaddr *) &name, &len)) < 0) throw AcceptException(errno);

  return ClientSocket(cs);
}

void ServerSocket::flush() {
  ::fsync(serversocket);
}

void ServerSocket::close() {
  state = NOT_CONNECTED;

#ifndef __unix
  ::close(serversocket);
#else
  ::shutdown(serversocket, 0);
#endif
}





