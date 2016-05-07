/*
 * Copywrite 2014-2015 Krzysztof Stasik. All rights reserved.
 */
#include "base/network/address.h"
#include "base/core/assert.h"

#include <netdb.h>

namespace Base {

const AddressIPv4 AddressIPv4::kLocalhost(127, 0, 0, 1);
const AddressIPv4 AddressIPv4::kAny(0, 0, 0, 0);

bool AddressIPv4::FromString(AddressIPv4 *address, const char *hostname) {
  BASE_ASSERT(address);
  struct hostent *value = gethostbyname(hostname);
  if(!value) {
    return false;
  }
  address->m_address = ntohl(((struct in_addr *)value->h_addr_list[0])->s_addr);
  return true;
}

s32 NetToHostL(s32 value) { return ntohl(value); }
s16 NetToHostS(s16 value) { return ntohs(value); }
s32 HostToNetL(s32 value) { return htonl(value); }
s16 HostToNetS(s16 value) { return ntohl(value); }

} // namespace Base
