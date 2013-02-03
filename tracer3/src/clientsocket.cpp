#include "clientsocket.h"

ClientSocket::ClientSocket() {
  state = NOT_CONNECTED;
}

ClientSocket::ClientSocket(SOCKET _clientsocket) {
  state = CONNECTED;
  clientsocket = _clientsocket;
}

ClientSocket::ClientSocket(const ClientSocket& c) {
  state = CONNECTED;
  clientsocket = c.clientsocket;
}

ClientSocket::ClientSocket(char* _hostname, u_short _port) : port(_port) {
  state = NOT_CONNECTED;
  strcpy(hostname, _hostname);
}

ClientSocket::~ClientSocket() {
  if (state != NOT_CONNECTED) {
    //close();
  }
}

int ClientSocket::read(char* buffer, int length, int flags) {
  int status;

  status = recv(clientsocket, buffer, length, flags);


  if (status > 0) {
    buffer[status] = '\0';
    //    std::cerr << getpid() << ": recieved " << status  << " bytes '" << buffer << "'" << std::endl << std:flush;
    
    return status;
  } else {
    state = NOT_CONNECTED;
    if (status == 0) throw NotConnectedException(errno);
    else switch (errno) {
      case ECONNRESET : throw ConnectionResetException(errno); break;
      case ENOTCONN   : throw NotConnectedException(errno); break;
      case ETIMEDOUT  : throw ConnectionTimedOutException(errno); break;
      case EAGAIN     : throw NoDataException(errno); break;
      case EIO        : throw ConnectionIOErrorException(errno); break;
    default: throw SocketException(errno); break;
    }
  }
}

int ClientSocket::write(char* buffer) {
  return write(buffer, strlen(buffer));
}

int ClientSocket::write(char* buffer, int length, int flags) {
  int status;

  status = send(clientsocket, buffer, length, flags);

  //std::cerr << getpid() << ": wrote " << status << " bytes" << std::endl << std::flush;

  if (status >= 0) return status;
  else {
    state = NOT_CONNECTED;
    if (status == 0) throw NotConnectedException(errno);
    else switch (errno) {
      case ECONNRESET : throw ConnectionResetException(errno); break;
      case ENOTCONN   : throw NotConnectedException(errno); break;
      case ETIMEDOUT  : throw ConnectionTimedOutException(errno); break;
      case EAGAIN     : throw NoDataException(errno); break;
      case EIO        : throw ConnectionIOErrorException(errno); break;
    default: throw SocketException(errno); break;    
    }
  }
}

int ClientSocket::connect() {
  if (state == CONNECTED) close();

  if ((hp = gethostbyname(hostname)) == NULL) throw HostNotFoundException(errno);

  if ((clientsocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) throw SocketCreationException(errno);

  memset(&name, 0, sizeof(struct sockaddr_in));

  name.sin_family = AF_INET;
  name.sin_port = htons(port);
  memcpy(&name.sin_addr, hp->h_addr_list[0], hp->h_length);
  len = sizeof(struct sockaddr_in);

  if (::connect(clientsocket, (struct sockaddr *) &name, len) < 0) throw ConnectionException(errno);
  state = CONNECTED;

  return 0;
}


void ClientSocket::flush() {
  ::fsync(clientsocket);
}

void ClientSocket::close() {
  state = NOT_CONNECTED;
  ::close(clientsocket);
}

void ClientSocket::shutdown(int howto) {
  state = NOT_CONNECTED;

#ifndef __unix
  ::close(clientsocket);
#else
  ::shutdown(clientsocket, 0);
#endif
}

ClientSocket& ClientSocket::operator = (ClientSocket& cs) {
  strcpy(hostname, cs.hostname);
  clientsocket = cs.clientsocket;
  port = cs.port;
  state = cs.state;

  return *this;
}



