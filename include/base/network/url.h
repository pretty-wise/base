/*
 * Copywrite 2014-2015 Krzysztof Stasik. All rights reserved.
 */
#pragma once

#include "base/network/address.h"
#include "base/core/types.h"

#define PRINTF_URL(url) url.GetAddress().GetA(), url.GetAddress().GetB(), url.GetAddress().GetC(), url.GetAddress().GetD(), url.GetPort()
 
namespace Base {

class Url
{
public:
		Url() : m_address((u32)0), m_port(0) {}
		Url( const AddressIPv4& address, u16 port ) : m_address(address), m_port(port) {}
		Url( const char* string );
		Url( const char* address, u16 port );

		AddressIPv4 GetAddress() const { return m_address; }
		u16 GetPort() const { return m_port; }
		void SetAddress(const AddressIPv4& address) { m_address = address; }
		void SetPort(u16 port) { m_port = port; }

		inline bool operator==( const Url& rhs ) const { return m_address == rhs.m_address && m_port == rhs.m_port; }
		inline bool operator!=( const Url& rhs ) const { return m_address != rhs.m_address || m_port != rhs.m_port; }

private:
		AddressIPv4 m_address;
		u16 m_port;
};

} // namespace Base
