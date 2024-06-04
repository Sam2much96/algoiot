/*
  Base32.h - Library for encoding to/decoding from Base32.
  Compatible with RFC 4648 ( http://tools.ietf.org/html/rfc4648 )
  Created by Vladimir Tarasow, December 18, 2012.
  Released into the public domain.
*/
// Documentation : https://github.com/NetRat/Base32/blob/master/Base32.h

#ifndef Base32_h
#define Base32_h

//#include "Arduino.h"
#include "stdint.h"
#include <stdlib.h>
// Boolean
#include <stdbool.h>
class Base32
{
  public:
    //Base32();
    static int toBase32(uint8_t *in, const int length , uint8_t *&out);
    static int toBase32(uint8_t *in, const int length, uint8_t *& out, bool);
    static int fromBase32(uint8_t *in, const int length, uint8_t *&out);
};

#endif