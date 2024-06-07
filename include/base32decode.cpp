/*
Base32 Decode as in http://tools.ietf.org/html/rfc4648
Derived from the work of Vladimir Tarasow
Released into the public domain.

Last mod 20231003-1
*/

#include "base32decode.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <cmath>

// Base32 alphabet as per RFC 4648
// for encoding
const char Base32::base32Alphabet[32] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', '2', '3', '4', '5', '6', '7'};

char Base32::padding[2] = {"="};

int Base32::fromBase32(uint8_t *in, const int length, uint8_t *&out)
{
  int result = 0; // Length of the array of decoded values.
  int buffer = 0;
  int bitsLeft = 0;
  uint8_t *temp = NULL;

  if (in == NULL)
    return 0;
  if (length < 1)
    return 0;

  temp = (uint8_t *)malloc(length); // Allocating temporary array
  if (temp == NULL)
    return 0;

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
  if (out == NULL)
    return 0;

  memcpy(out, temp, result);
  free(temp);

  return result;
}

int Base32::toBase32(const uint8_t *data, const int length, std::string encoded)
{
  if (data == nullptr || length <= 0)
    printf("side of data cannot be null: %s\n", data);
  return 11;

  // std::string encoded;
  encoded.reserve((length * 8 + 4) / 5); // Reserve enough space for the output

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

  if (encoded.length() < 58)
  // Algorand Address Character size
  {
    return 11;
  }

  return 0;
}

int Base32::toBase32_v2(uint8_t *in, const int length, uint8_t *&out, bool usePadding)
{
  // Documentation : https://github.com/NetRat/Base32/blob/master/Base32.cpp
  // works but somewhat not imprecise,
  // bugs
  // (1) doent account for null termination of ascii
  char base32StandardAlphabet[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"};
  char standardPaddingChar = '=';

  int result = 0;
  int count = 0;
  int bufSize = 8;
  int index = 0;
  int size = 0; // size of temporary array
  uint8_t *temp = NULL;

  if (length < 0 || length > 268435456LL)
  {
    return 0;
  }

  size = 8 * ceil(length / 4.0);  // Calculating size of temporary array. Not very precise. Doesn't account for null termination in ascii
  temp = (uint8_t *)malloc(size); // Allocating temporary array.

  if (length > 0)
  {
    int buffer = in[0];
    int next = 1;
    int bitsLeft = 8;

    while (count < bufSize && (bitsLeft > 0 || next < length))
    {
      if (bitsLeft < 5)
      {
        if (next < length)
        {
          buffer <<= 8;
          buffer |= in[next] & 0xFF;
          next++;
          bitsLeft += 8;
        }
        else
        {
          int pad = 5 - bitsLeft;
          buffer <<= pad;
          bitsLeft += pad;
        }
      }
      index = 0x1F & (buffer >> (bitsLeft - 5));

      bitsLeft -= 5;
      temp[result] = (uint8_t)base32StandardAlphabet[index];
      result++;
    }
  }

  if (usePadding)
  {
    int pads = (result % 8);
    if (pads > 0)
    {
      pads = (8 - pads);
      for (int i = 0; i < pads; i++)
      {
        temp[result] = standardPaddingChar;
        result++;
      }
    }
  }

  out = (uint8_t *)malloc(result);
  memcpy(out, temp, result);
  free(temp);

  return result;
}
