/*
 * Copywrite 2014-2015 Krzysztof Stasik. All rights reserved.
 */
#include "base/network/url.h"

#include <string>
#include <stdlib.h>

namespace Base {

Url::Url() {
  m_hostname[0] = '\0';
  m_service[0] = '\0';
}

Url::Url(const AddressIPv4 &address, u16 port) {
  snprintf(m_service, kServiceMax, "%d", port);
  snprintf(m_hostname, kHostnameMax, "%d.%d.%d.%d", address.GetA(),
           address.GetB(), address.GetC(), address.GetD());
}

Url::Url(const char *string) {
  std::string address = string;

  std::size_t a_pos = 0;
  std::size_t b_pos = address.find('.', a_pos) + 1;
  std::size_t c_pos = address.find('.', b_pos) + 1;
  std::size_t d_pos = address.find('.', c_pos) + 1;
  std::size_t p_pos = address.find(':', d_pos) + 1;

  strncpy(m_hostname, string, p_pos > kHostnameMax ? kHostnameMax : p_pos);
  m_hostname[kHostnameMax - 1] = '\0';
  strncpy(m_service, address.substr(p_pos).c_str(), kServiceMax);
  m_service[kServiceMax - 1] = '\0';
}

Url::Url(const char *string, u16 port) {
  strncpy(m_hostname, string, kHostnameMax);
  m_hostname[kHostnameMax - 1] = '\0';
  snprintf(m_service, kServiceMax, "%d", port);
}

Url::Url(const char *hostname, const char *service) {
  strncpy(m_hostname, hostname, kHostnameMax);
  m_hostname[kHostnameMax - 1] = '\0';
  strncpy(m_service, service, kServiceMax);
  m_hostname[kServiceMax - 1] = '\0';
}

Url::Url(const Socket::Address &addr) {
  addr.GetHostname(m_hostname, kHostnameMax);
  addr.GetService(m_service, kServiceMax);
}

u16 Url::GetPort() const { return atoi(m_service); }

} // namespace Base
