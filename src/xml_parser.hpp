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

#ifndef _XML_PARSER_HPP_
#define _XML_PARSER_HPP_

#define XML_HEX_VAL(_buf)    ((_buf[0] == '0' && _buf[1] == 'x'))

class XmlParser
{
   private:
      xml_document   m_xmlDoc;

      MsgTask* procSend(xml_node *node);      
      MsgTask* procRecv(xml_node *node);      
      MsgTask* procWait(xml_node *node);      
      RETVAL procIe(xml_node *node, GtpIeLst *pIeLst);
      RETVAL procStore(xml_node *node);      
      RETVAL procValidate(xml_node *node);      
      RETVAL procComplexIe(GtpIe *pIe, xml_node *pXmlIe);
      RETVAL procGroupedIe(GtpIe *pIe, xml_node *pXmlIe);

   public:
      MsgVec* parseXmlDoc();
      XmlParser(const S8* pXmlFile) throw (ErrCodeEn);

      ~XmlParser();
};


EXTERN VOID parseXmlScenario(const S8*, MsgVec*) throw (ErrCodeEn);

#endif
