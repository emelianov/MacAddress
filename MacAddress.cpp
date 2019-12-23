/*
  IPAddress.cpp - Base class that provides IPAddress
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

#include <Arduino.h>
#include <MacAddress.h>

MacAddress::MacAddress()
{
   MacAddress(0,0,0,0,0,0);
}

MacAddress::MacAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet, uint8_t fifth_octet, uint8_t sixth_octet)
{
    _mac[0] = first_octet;
    _mac[1] = second_octet;
    _mac[2] = third_octet;
    _mac[3] = fourth_octet;
    _mac[4] = fifth_octet;
    _mac[5] = sixth_octet;
}

MacAddress::MacAddress(const uint8_t *address)
{
    memcpy(_mac, address, sizeof(_mac));
}

bool MacAddress::fromString(const char *address)
{
    uint16_t acc = 0; // Accumulator
    uint8_t dots = 0;

    while (*address)
    {
        char c = *address++;
        if (isHex(c))
        {
            acc = acc * 16 + (c - '0');
            if (acc > 255) {
                // Value out of [0..255] range
                return false;
            }
        }
        else if (c == '-' || c == ':')
        {
            if (dots == 5) {
                // Too much separators (there must be 5)
                return false;
            }
            _mac[dots++] = acc;
            acc = 0;
        }
        else
        {
            // Invalid char
            return false;
        }
    }

    if (dots != 5) {
        // Too few dots (there must be 3 dots)
        return false;
    }
    _mac[5] = acc;
    return true;
}

MacAddress& MacAddress::operator=(const uint8_t *address)
{
    memcpy(_mac, address, sizeof(_mac));
    return *this;
}

bool MacAddress::operator==(const uint8_t* addr) const
{
    return memcmp(addr, _mac, sizeof(_mac)) == 0;
}

size_t MacAddress::printTo(Print& p) const
{
    size_t n = 0;
    for (int i =0; i < 5; i++)
    {
        n += p.print(_mac[i], HEX);
        n += p.print('-');
    }
    n += p.print(_mac[5], HEX);
    return n;
}

