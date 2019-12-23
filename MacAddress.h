/*
  IPAddress.h - Base class that provides IPAddress
  Copyright (c) 2011 Adrian McEwen.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#include <stdint.h>
#include "Printable.h"
#include "WString.h"

// A class to make it easier to handle and pass around IP addresses

class MacAddress : public Printable {
private:
	uint8_t _mac[6];

    // Access the raw byte array containing the address.  Because this returns a pointer
    // to the internal structure rather than a copy of the address this function should only
    // be used when you know that the usage of the returned uint8_t* will be transient and not
    // stored.
    uint8_t* raw_address() { return _mac; };
    bool isHex(char h) {
      return (h=='0'||h=='1'||h=='2'||h=='3'||h=='4'||h=='5'||h=='6'||h=='7'||h=='8'||h=='9'||h=='A'||h=='B'||h=='C'||h=='D'||h=='E'||h=='F'||h=='a'||h=='b'||h=='c'||h=='d'||h=='e'||h=='f');
    }
    char* _str = nullptr;

public:
    // Constructors
    MacAddress();
    MacAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet, uint8_t fifth_octet, uint8_t sixth_octet);
    MacAddress(const uint8_t *address);
    ~MacAddress() { free(_str); }

    bool fromString(const char *address);
    bool fromString(const String &address) { return fromString(address.c_str()); }

    // Overloaded cast operator to allow IPAddress objects to be used where a pointer
    // to a four-byte uint8_t array is expected
    bool operator==(const MacAddress& addr);
    bool operator==(const uint8_t* addr) const;

    // Overloaded index operator to allow getting and setting individual octets of the address
    uint8_t operator[](int index) const { return _mac[index]; };
    uint8_t& operator[](int index) { return _mac[index]; };

    operator char*() {
      if (!_str) {
        _str = (char*)malloc(18);
        sprintf(_str, "%02X-02X-02X-02X-02X-02X", _mac[0], _mac[1], _mac[2], _mac[3], _mac[4], _mac[5]);
      }
      return _str;
    }

    // Overloaded copy operators to allow initialisation of IPAddress objects from other types
    MacAddress& operator=(const uint8_t *address);

    virtual size_t printTo(Print& p) const;

};

const MacAddress MACADDR_NONE(0,0,0,0,0,0);
