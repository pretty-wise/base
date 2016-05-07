/*
 * Copywrite 2014-2015 Krzysztof Stasik. All rights reserved.
 */
#include "base/network/url.h"

#include <string>
#include <cstdlib>

namespace Base {

Url::Url(const char *string) {
  std::string address = string;

  std::size_t a_pos = 0;
  std::size_t b_pos = address.find('.', a_pos) + 1;
  std::size_t c_pos = address.find('.', b_pos) + 1;
  std::size_t d_pos = address.find('.', c_pos) + 1;
  std::size_t p_pos = address.find(':', d_pos) + 1;

  m_address = AddressIPv4(
      atoi(address.substr(a_pos).c_str()), atoi(address.substr(b_pos).c_str()),
      atoi(address.substr(c_pos).c_str()), atoi(address.substr(d_pos).c_str()));
  m_port = atoi(address.substr(p_pos).c_str());
}

Url::Url(const char *string, u16 port) {
  std::string address = string;

  std::size_t a_pos = 0;
  std::size_t b_pos = address.find('.', a_pos) + 1;
  std::size_t c_pos = address.find('.', b_pos) + 1;
  std::size_t d_pos = address.find('.', c_pos) + 1;

  m_address = AddressIPv4(
      atoi(address.substr(a_pos).c_str()), atoi(address.substr(b_pos).c_str()),
      atoi(address.substr(c_pos).c_str()), atoi(address.substr(d_pos).c_str()));
  m_port = port;
}

} // namespace Base
