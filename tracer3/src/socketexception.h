#ifndef __SOCKETEXCEPTIONS_H__
#define __SOCKETEXCEPTIONS_H__

#include <string>

class SocketException {
 public:
  SocketException(int _i) : msg("SocketException"), i(_i) {
  }

  SocketException(std::string _msg, int _i) : msg(_msg), i(_i) {
  }

  std::string getMessage() {
    return msg + ": " + getErrnoString(i);
  }

  int getErrno() {
    return i;
  }

  std::string getErrnoString(int i) {
    if (i == EBADF) return "EBADF";
    if (i == EINTR) return "EINTR";
    if (i == EMFILE) return "EMFILE";
    if (i == ENODEV) return "ENODEV";
    if (i == ENOMEM) return "ENOMEM";
    if (i == ENOTSOCK) return "ENOTSOCK";
    if (i == EPROTO) return "EPROTO";
    if (i == EWOULDBLOCK) return "EWOULDBLOCK";

    return "UNKNOWN";
  }

 protected:
  std::string msg;
  int i;
};


class HostNotFoundException : public SocketException {
public:
  HostNotFoundException(int _i) : SocketException("HostNotFoundException", _i) { }
};


class NotConnectedException : public SocketException {
public:
  NotConnectedException(int _i) : SocketException("NotConnectedException", _i) { }
};


class ConnectionResetException :  public SocketException {
public:
  ConnectionResetException(int _i) : SocketException("ConnectionResetException", _i) { }
};


class ConnectionTimedOutException :  public SocketException {
public:
  ConnectionTimedOutException(int _i) : SocketException("ConnectionTimedOutException", _i) { }
};


class ConnectionException : public SocketException {
public:
  ConnectionException(int _i) : SocketException("ConnectionException", _i) { }
};


class ConnectionIOErrorException :  public SocketException {
public:
  ConnectionIOErrorException(int _i) : SocketException("ConnectionIOErrorException", _i) { }
};


class NoDataException :  public SocketException {
public:
  NoDataException(int _i) : SocketException("NoDataException", _i) { }
};


class SocketCreationException : public SocketException {
public:
  SocketCreationException(int _i) : SocketException("SocketCreationException", _i) { }
};


class BindException : public SocketException {
public:
 BindException(int _i) : SocketException("BindException", _i) { }
};

class AcceptException : public SocketException {
public:
  AcceptException(int _i) : SocketException("AcceptException", _i) { }
};

class ListenException : public SocketException {
public:
  ListenException(int _i) : SocketException("ListenException", _i) { }
};

  
#endif
