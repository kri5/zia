#ifndef ICLIENT_SOCKET_H__
#define ICLIENT_SOCKET_H__

#include <string>

namespace zAPI
{
    /// Interface for hooking socket creation, after accept()
    class IClientSocket
    {
        public:

            /**
             *  The interface virtual destructor
             */
            virtual ~IClientSocket(){}

            /**
             *  Returns the size that has been read on the socket and
             *  fills the buffer passed in parameter with the received data
             *  that will never been greater than the specified length.
             *  @param buf a buffer
             *  @param length the maximum length to read
             *  @return the read length
             */
            virtual int	    recv(char *buf, int length) const = 0;

            /**
             *  Sends a certain amount of data contained in the specified 
             *  buffer (as a char*) through the socket.
             *  @param buf the buffer to send
             *  @param length the wanted size to send
             *  @return the size sent through the socket
             */
            virtual int     send(const char *buf, int length) const = 0;

            /**
             *  Sends a certain amount of data contained in the specified 
             *  buffer (as a std::string) through the socket.
             *  @param buf the buffer to send
             *  @param length wanted size to send (or -1, if you want to send the entire string)
             *  @return the size sent through the socket
             */
            virtual int     send(const std::string& buf, int length) const = 0;

            /**
             *  Close the socket, and eventually shut it down nicelly.
             *
             *  @param shutdown If true, close will call shutdown(2)
             */
            virtual void    close(bool shutdown) = 0;

            /**
             *  Check if socket has already be closed, to prevent multiple socket
             *  closure.
             *
             *  @return a boolean indicating if socket has been closed.
             */

            virtual bool    isClosed() const = 0;
            /**
             * Get the underlying socket used for the client
             * @return a socket identifier
             */
            
            virtual int	    getNativeSocket() const = 0;

    };
}

#endif //ICLIENT_SOCKET_H__

