/*
  Base32.cpp - Library for encoding to/decoding from Base32.
  Compatible with RFC 4648 ( http://tools.ietf.org/html/rfc4648 )
  Created by Vladimir Tarasow, December 18, 2012.
  Released into the public domain.
*/
#include "Arduino.h"
#include "Base32.h"
#include "stdint.h"
#include <string.h>
#include <cmath>
#include <stdlib.h>
// Base32::Base32()
//{
// }

// int Base32::toBase32(uint8_t *in, const int length, uint8_t *&out)
//{
//     return toBase32(in, length, out, false);
// }

std::string Base32::toBase32(const uint8_t *data, const int length, std::string encoded)
{
    if (data == nullptr || length <= 0)
        return "";

    // std::string encoded;
    encoded.reserve((length + 4) / 5 * 8); // Reserve enough space for the output

    int buffer = 0;
    int bitsLeft = 0;

    for (int i = 0; i < length; ++i)
    {
        buffer <<= 8;
        buffer |= data[i] & 0xFF;
        bitsLeft += 8;

        while (bitsLeft >= 5)
        {
            encoded += base32Alphabet[(buffer >> (bitsLeft - 5)) & 0x1F];
            bitsLeft -= 5;
        }
    }

    if (bitsLeft > 0)
    {
        buffer <<= (5 - bitsLeft);
        encoded += base32Alphabet[buffer & 0x1F];
    }

    // Padding to make the output length a multiple of 8
    while (encoded.size() % 8 != 0)
    {
        encoded += '=';
    }

    return encoded;
}

int Base32::fromBase32(uint8_t *in, const int length, uint8_t *&out)
{
    int result = 0; // Length of the array of decoded values.
    int buffer = 0;
    int bitsLeft = 0;
    uint8_t *temp = NULL;

    temp = (uint8_t *)malloc(length); // Allocating temporary array.

    for (int i = 0; i < length; i++)
    {
        uint8_t ch = in[i];

        // ignoring some characters: ' ', '\t', '\r', '\n', '='
        if (ch == 0xA0 || ch == 0x09 || ch == 0x0A || ch == 0x0D || ch == 0x3D)
            continue;

        // recovering mistyped: '0' -> 'O', '1' -> 'L', '8' -> 'B'
        if (ch == 0x30)
        {
            ch = 0x4F;
        }
        else if (ch == 0x31)
        {
            ch = 0x4C;
        }
        else if (ch == 0x38)
        {
            ch = 0x42;
        }

        // look up one base32 symbols: from 'A' to 'Z' or from 'a' to 'z' or from '2' to '7'
        if ((ch >= 0x41 && ch <= 0x5A) || (ch >= 0x61 && ch <= 0x7A))
        {
            ch = ((ch & 0x1F) - 1);
        }
        else if (ch >= 0x32 && ch <= 0x37)
        {
            ch -= (0x32 - 26);
        }
        else
        {
            free(temp);
            return 0;
        }

        buffer <<= 5;
        buffer |= ch;
        bitsLeft += 5;
        if (bitsLeft >= 8)
        {
            temp[result] = (unsigned char)((unsigned int)(buffer >> (bitsLeft - 8)) & 0xFF);
            result++;
            bitsLeft -= 8;
        }
    }

    out = (uint8_t *)malloc(result);
    memcpy(out, temp, result);
    free(temp);

    return result;
}