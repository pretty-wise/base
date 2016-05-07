/*
 * Copywrite 2014-2015 Krzysztof Stasik. All rights reserved.
 */
#pragma once

#include "base/core/types.h"
#include "base/network/url.h"

#include <sys/socket.h>
#include <errno.h>

namespace Base {

namespace Socket {

typedef int Handle;
static const Handle InvalidHandle = -1;

const char *ErrorToString(int error);

void Close(Handle socket);

bool GetNetworkInterface(Base::AddressIPv4 *address,
                         const char *interface_name);

bool GetBoundAddress(Handle socket, Base::Url *url);

namespace Tcp {
static const int kFailed = 0;
static const int kConnecting = 1;
static const int kConnected = 2;

Handle Open();
bool Listen(Handle socket, u16 *port);
int Connect(Handle socket, const Base::Url &url);
int IsConnected(Handle socket);

bool Accept(Handle socket, Handle *incoming, Base::Url *connectee);
streamsize Send(Handle socket, const void *data, streamsize nbytes, int *error);
streamsize Recv(Handle socket, void *buffer, streamsize nbytes, int *error);

} // namespace Tcp

namespace Udp {

Handle Open(u32 addr, u16 *port);
inline Handle Open(u16 *port) { return Open(0, port); }
bool Send(Handle socket, const Base::Url &url, const void *buffer,
          streamsize nbytes, int *error);
streamsize Recv(Handle socket, Base::Url *url, void *buffer, streamsize nbytes,
                int *error);

} // namespace Udp
} // namespace Socket
} // namespace Base
