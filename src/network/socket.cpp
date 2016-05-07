/*
 * Copywrite 2014-2015 Krzysztof Stasik. All rights reserved.
 */
//
//	Socket.cpp
//	Base
//
//	Created by Krzysiek Stasik on 05/07/14.
//	Copyright (c) 2014 Krzysiek Stasik. All rights reserved.
//

#include "base/network/socket.h"
#include "base/core/assert.h"

#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <unistd.h>

namespace Base {
namespace Socket {

void Close(Handle socket) { close(socket); }

const char *ErrorToString(int error) {
  switch(error) {
  case EACCES:
    return "EACCES";
  case EAGAIN:
    return "EAGAIN";
  // case EWOULDBLOCK: return "EWOULDBLOCK";
  case EBADF:
    return "EBADF";
  case ECONNRESET:
    return "ECONNRESET";
  case EDESTADDRREQ:
    return "EDESTADDRREQ";
  case EFAULT:
    return "EFAULT";
  case EINTR:
    return "EINTR";
  case EINVAL:
    return "EINVAL";
  case EISCONN:
    return "EISCONN";
  case EMSGSIZE:
    return "EMSGSIZE";
  case ENOBUFS:
    return "ENOBUFS";
  case ENOMEM:
    return "ENOMEM";
  case ENOTCONN:
    return "ENOTCONN";
  case ENOTSOCK:
    return "ENOTSOCK";
  case EOPNOTSUPP:
    return "EOPNOTSUPP";
  case EPIPE:
    return "EPIPE";
  }
  return "UNKNOWN";
}

bool GetBoundAddress(Handle socket, Base::Url *url) {
  struct sockaddr_in address;
  memset(&address, 0, sizeof(sockaddr));
  socklen_t addr_len = sizeof(address);
  int res = getsockname(socket, (struct sockaddr *)&address, &addr_len);

  if(res == -1 || addr_len != sizeof(address)) {
    return false;
  }

  url->SetAddress(Base::AddressIPv4((u32)ntohl(address.sin_addr.s_addr)));
  url->SetPort(ntohs(address.sin_port));
  return true;
}

bool GetNetworkInterface(Base::AddressIPv4 *address, const char *if_name) {
  struct ifaddrs *ifap;
  bool result = false;
  if(-1 == getifaddrs(&ifap)) {
    return false;
  }

  // todo(kstasik): very naive way, binds to first AF_INET eth0 interface...
  for(struct ifaddrs *ifa = ifap; ifa != nullptr; ifa = ifa->ifa_next) {
    if(ifa->ifa_addr == nullptr)
      continue;
    if(!(ifa->ifa_flags & IFF_UP))
      continue;

    if(ifa->ifa_addr->sa_family == AF_INET) {
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)ifa->ifa_addr;
      if(result == false &&
         (0 == strncmp(ifa->ifa_name, if_name, strlen(if_name)))) {
        result = true;
        *address = Base::AddressIPv4((u32)ntohl(ipv4->sin_addr.s_addr));
      }
    }
  }

  freeifaddrs(ifap);
  return result;
}

namespace Tcp {

Handle Open() {
  Handle handle = socket(PF_INET, SOCK_STREAM, 0);
  if(handle == InvalidHandle)
    return handle;

  // reuse address
  const int yes = 1;
  if(-1 == setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))) {
    Close(handle);
    return InvalidHandle;
  }

  // non-blocking socket.
  if(-1 == fcntl(handle, F_SETFL, O_NONBLOCK)) {
    Close(handle);
    return InvalidHandle;
  }
  return handle;
}

bool Listen(Handle socket, u16 *port) {
  if(socket == InvalidHandle) {
    return false;
  }

  struct sockaddr_in address;
  memset(&address, 0, sizeof(sockaddr));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(*port);

  int res = bind(socket, (const struct sockaddr *)&address, sizeof(address));

  if(res == -1) {
    BASE_LOG("bind on port %d failed with %d\n", *port, errno);
    return false;
  }

  socklen_t addr_len = sizeof(address);
  res = getsockname(socket, (struct sockaddr *)&address, &addr_len);

  if(res == -1)
    return false;

  *port = ntohs(address.sin_port);

  res = listen(socket, 5);

  if(res == -1)
    return false;

  return true;
}

int Connect(Handle socket, const Base::Url &url) {
  struct sockaddr_in address;
  memset(&address, 0, sizeof(sockaddr));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(url.GetAddress().GetRaw());
  address.sin_port = htons(url.GetPort());

  int result = connect(socket, (struct sockaddr *)&address, sizeof(address));
  BASE_ASSERT(result == 0 || (result == -1 && errno == EINPROGRESS),
              "socket connect failed %d. errno: %d.", result, errno);
  if(result == 0) {
    return kConnected;
  } else if(result == -1 && errno == EINPROGRESS) {
    return kConnecting;
  }
  return kFailed;
  // return result == 0 || (result == -1 && errno == EINPROGRESS);
}

int IsConnected(Handle socket) {
  struct timeval timeout = {0, 0};
  fd_set read_set;
  FD_ZERO(&read_set);
  FD_SET(socket, &read_set);

  int result = select(socket + 1, nullptr, &read_set, nullptr, &timeout);
  if(result < 0 && errno != EINTR) {
    return kFailed;
  } else if(result > 0) {
    int valopt;
    socklen_t len = sizeof(int);
    if(getsockopt(socket, SOL_SOCKET, SO_ERROR, (void *)&valopt, &len) < 0) {
      BASE_LOG_LINE("getsockopt %d", valopt);
      if(valopt == EINPROGRESS) {
        return kConnecting;
      } else if(valopt == 0) {
        return kConnected;
      } else {
        return kFailed;
      }
    }
  } else {
    return kConnecting;
  }

  if(result == 1) {
    return kConnected;
  }
  return kFailed;
}

bool Accept(Handle socket, Handle *incoming, Base::Url *connectee) {
  struct sockaddr_in address;
  socklen_t address_len = sizeof(address);
  memset(&address, 0, sizeof(sockaddr));

  int result = accept(socket, (struct sockaddr *)&address, &address_len);
  if(-1 != result) {

    if(-1 == fcntl(result, F_SETFL, O_NONBLOCK)) {
      Close(result);
      return false;
    }

    const int yes = 1;
    if(-1 == setsockopt(result, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))) {
      Close(result);
      return false;
    }

    *incoming = result;

    connectee->SetAddress(
        Base::AddressIPv4((u32)ntohl(address.sin_addr.s_addr)));
    connectee->SetPort(ntohs(address.sin_port));
  }

  return result != -1;
}

streamsize Send(Handle socket, const void *data, streamsize nbytes,
                int *error) {
  streamsize result = send(socket, data, nbytes, 0);
  if(result == -1) {
    *error = errno;
  }
  return result;
}

streamsize Recv(Handle socket, void *buffer, streamsize nbytes, int *error) {
  streamsize nbytes_read = read(socket, buffer, nbytes);
  if(nbytes_read == -1) {
    if(errno != EWOULDBLOCK) {
      BASE_LOG_LINE("socket read on %d failed with %d", socket, errno);
    }
    *error = errno;
  }
  return nbytes_read;
}

} // namespace Tcp

namespace Udp {

Handle Open(u32 address, u16 *port) {
  struct sockaddr_in my_addr;
  memset(&my_addr, 0, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(*port);
  my_addr.sin_addr.s_addr = htonl(address);

  Handle handle = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(handle == InvalidHandle) {
    BASE_LOG_LINE("socket failed with %d", errno);
    return InvalidHandle;
  }
  if(-1 == bind(handle, (struct sockaddr *)&my_addr, sizeof(my_addr))) {
    BASE_LOG_LINE("bind failed with %d", errno);
    Close(handle);
    return InvalidHandle;
  }

  // non-blocking socket.
  if(-1 == fcntl(handle, F_SETFL, O_NONBLOCK)) {
    BASE_LOG_LINE("fcntl failed with %d", errno);
    Close(handle);
    return InvalidHandle;
  }

  struct sockaddr_in addr;
  socklen_t addr_len = sizeof(addr);
  int res = getsockname(handle, (struct sockaddr *)&addr, &addr_len);

  if(res == -1) {
    BASE_LOG_LINE("getsockname failed with %d", errno);
    Close(handle);
    return InvalidHandle;
  }

  *port = ntohs(addr.sin_port);

  return handle;
}

bool Send(Handle socket, const Base::Url &dest, const void *buffer,
          streamsize nbytes, int *error) {
  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(dest.GetAddress().GetRaw());
  address.sin_port = htons(dest.GetPort());
  streamsize sent = sendto(socket, buffer, nbytes, 0, (sockaddr *)&address,
                           sizeof(sockaddr_in));
  if(sent == -1) {
    *error = errno;
    return false;
  }
  return true;
}

streamsize Recv(Handle socket, Base::Url *from, void *buffer, streamsize nbytes,
                int *error) {
  sockaddr_in from_;
  socklen_t fromsize = sizeof(from_);
  streamsize received = recvfrom(socket, (char *)buffer, nbytes, 0,
                                 (sockaddr *)&from_, &fromsize);
  if(received == -1) {
    if(errno == EWOULDBLOCK) {
      return 0;
    }
    BASE_LOG_LINE("socket read on %d failed with %d", socket, errno);
    *error = errno;
  }
  u32 address = ntohl(from_.sin_addr.s_addr);
  u16 port = ntohs(from_.sin_port);
  *from = Base::Url(Base::AddressIPv4(address), port);
  return received;
}

} // namespace Udp

} // namespace Socket
} // namespace Base
