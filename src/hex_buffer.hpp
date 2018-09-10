/*  Copyright (C) 2013  Nithin Nellikunnu, nithin.nn@gmail.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _HEX_BUFFER_H_
#define _HEX_BUFFER_H_

#include <math.h>

#include <error.hpp>
#include <gtp_util.hpp>

class HexBuffer
{
public:
    HexBuffer(const char *input)
    {
        if (input[0] == '0' && input[1] == 'x')
        {
            size   = ceil(STRLEN(input) / 2);
            buffer = new uint8_t[size];

            /* Skipping the 0x part of hex string */
            uint32_t begin = 2;
            uint32_t indx  = 2;

            /* hex buffer string has odd len, so the first byte of hex buffer
             * will have 0 as High nibble
             */
            if (GSIM_IS_ODD(STRLEN(input)))
            {
                buffer[indx++] = 0x0f & gtpCharToHex(input[0]);
                begin++;
            }

            for (uint32_t i = begin; i < STRLEN(input); i += 2)
            {
                U8 HNibble     = gtpCharToHex(input[i]);
                U8 LNibble     = gtpCharToHex(input[i + 1]);
                buffer[indx++] = (HNibble << 4) | (LNibble);
            }
        }
        else
        {
            std::string message = "Invalid hex string";
            message += input;
            throw GsimError(message);
        }
    }

    virtual ~HexBuffer()
    {
        delete[] buffer;
        size = 0;
    }

    Buffer *getBuffer()
    {
        Buffer *buf = new Buffer();
        buf->len    = size;
        MEMCPY(buf->pVal, buffer, size);
        return buf;
    }

private:
    uint8_t *buffer;
    uint32_t size;
};

#endif
