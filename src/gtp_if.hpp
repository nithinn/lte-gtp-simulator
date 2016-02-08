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

#ifndef _GTP_IF_HPP_
#define _GTP_IF_HPP_

class   GtpIe;
typedef std::list<GtpIe *>       GtpIeLst;
typedef GtpIeLst::iterator       GtpIeLstItr;

class GtpIe
{
   protected:
      GtpIeHdr    m_hdr;

   public:
      /* builds IE from a char string */
      virtual RETVAL buildIe(const S8 *pVal) = 0;

      /* build IE from a hex string */
      virtual RETVAL buildIe(const HexString *value) = 0;

      /* builds ie from list of buffers, used for complex IEs (e.g. ULI) */
      virtual RETVAL buildIe(IeParamLst *pBuf) = 0;

      /* builds is from a list of IEs, Grouped IEs */
      virtual RETVAL buildIe(const GtpIeLst *pIeLst) = 0;

      /* encodes IE header and Contents into a byte buffer pointed by pBuf
       * returns the length of encoded buffer
       */
      virtual GtpLength_t encode(U8 *pBuf) = 0;

      /* decodes the IE from the byte buffer, updates the ie header length
       * and copies the ie contents into local buffer
       */
      virtual GtpLength_t decode(const U8 *ieBuf) = 0;

      virtual BOOL   isGroupedIe() = 0;

      virtual ~GtpIe() {};

      // the factory method returns Gtp IE instance
      static GtpIe*  createGtpIe(GtpIeType_t ieType, GtpInstance_t instance);

      U8 *getIeBufPtr(U8*, GtpLength_t, GtpIeType_t, GtpInstance_t, U32);

      GtpIeType_t    type() {return m_hdr.ieType;}

      GtpInstance_t  instance() {return m_hdr.instance;}

   protected:

      /* Helper function used by derived IE classes to encode IE
       * into byte buffer
       */
      GtpLength_t encodeHelper(U8 *inbuf, U8 *outbuf);

      /* Helper function used by derived IE classes to decode IE from
       * byte buffer received into member variable
       */
      GtpLength_t decodeHelper(const U8 *inbuf, U8 *outbuf,\
            GtpLength_t maxIeLen);

      /* Helper function used by derived IE classes to build IE from
       * hex string read from xml scenario file 
       */
      RETVAL buildIeHelper(const HexString *inbuf, U8 *outbuf,\
            GtpLength_t maxIeLen);

};

#endif
