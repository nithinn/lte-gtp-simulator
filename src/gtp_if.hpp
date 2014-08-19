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
typedef std::list<XmlBuffer *>   XmlBufferLst;
typedef XmlBufferLst::iterator   XmlBufferLstItr;

class GtpIe
{
   protected:
      GtpIeHdr    hdr;

   public:
      virtual RETVAL encode(const S8 *pVal) = 0;
      virtual RETVAL decode(const Buffer* pBuf) = 0;
      virtual RETVAL encode(XmlBuffer* pBuf) = 0;
      virtual RETVAL encode(XmlBufferLst *pBuf) = 0;
      virtual RETVAL encode(const GtpIeLst *pIeLst) = 0;
      virtual RETVAL encode(U8 *pBuf, U32 *pLen) = 0;
      virtual BOOL   isGroupedIe() = 0;
      GtpIeType_E    type() {return hdr.ieType;}
      GtpInstance_t  instance() {return hdr.instance;}
      virtual ~GtpIe() {};

      static GtpIe*  createGtpIe(GtpIeType_E ieType, GtpInstance_t instance);
      U8*            getIeBufPtr(U8*, GtpLength_t, GtpIeType_E, GtpInstance_t,\
                                 U32);
};

#endif
