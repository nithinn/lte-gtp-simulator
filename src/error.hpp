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

#ifndef _ERROR_HPP_
#define _ERROR_HPP_

#include <exception>
#include <string>

typedef enum {
    ERR_NO,                 // no error
    ERR_UKNOWN,             // uknown error
    ERR_INV_CMD_LINE_PARAM, // invalid command line parameters
    ERR_XML_ARGS,
    ERR_XML_INIT,
    ERR_XML_PARSE,
    ERR_XML_EMPTY_DOCUMENT,
    ERR_SOCK_ALLOC,
    ERR_THREAD_CREATE,
    ERR_THREAD_ATTR_INIT,
    ERR_MEMORY_ALLOC,
    ERR_INV_TIMER_CONFIG,
    ERR_INV_SOCKET_TYPE,
    ERR_XML_PARSING,
    ERR_XML_PROCESSING,
    ERR_DISPLAY_INIT,
    ERR_KEYBOARD_INIT,
    ERR_SYS_FCNTL,
    ERR_SYS_SOCKET_BIND,
    ERR_SYS_SOCKET_CREATE,
    ERR_SYS_SOCK_READ,
    ERR_SYS_SOCK_SEND,
    ERR_SYS_SOCK_CNTRL,
    ERR_PDN_CREATION,
    ERR_CTUN_CREATION,
    ERR_GTP_MSG_BUF_OVERFLOW,
    ERR_RETRANS_GTPC_MSG_RCVD,
    ERR_UNEXPECTED_GTPC_MSG_RCVD,
    ERR_MAX_RETRY_EXCEEDED,
    ERR_IE_NOT_FOUND,
    ERR_INVALID_IE_LENGTH,
    ERR_MAX
} ErrCodeEn;

class GsimError : public std::exception
{
public:
    /**
     * Constructor (C strings).
     * @param message C-style string error message.
     *                 The string contents are copied upon construction.
     *                 Hence, responsibility for deleting the char* lies
     *                 with the caller.
     */
    explicit GsimError(const char *message) : msg_(message)
    {
    }

    /**
     * Constructor (C++ STL strings).
     * @param message The error message.
     */
    explicit GsimError(const std::string &message) : msg_(message)
    {
    }

    /**
     * Destructor.
     * Virtual to allow for subclassing.
     */
    virtual ~GsimError() throw()
    {
    }

    /**
     * Returns a pointer to the (constant) error description.
     * @return A pointer to a const char*. The underlying memory
     *          is in posession of the exception object. Callers must
     *          not attempt to free the memory.
     */
    virtual const char *what() const throw()
    {
        return msg_.c_str();
    }

protected:
    /**
     * Error message.
     */
    std::string msg_;
};

#endif
