#ifndef __ICLIENT_SOCKET_H__
#define __ICLIENT_SOCKET_H__

#include <string>

/// Interface for ClientSocket classes, to ensure multi-platform compactibility
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
		virtual int	recv(char *buf, int length) const = 0;

		/**
         *  Sends a certain amount of data contained in the specified 
         *  buffer (as a char*) through the socket.
         *  @param buf the buffer to send
         *  @param length wanted size to send (or -1 for the all buffer)
         *  @return the size sent through the socket
         */
		virtual int send(const char *buf, int length = -1) const = 0;

		/**
         *  Sends a certain amount of data contained in the specified 
         *  buffer (as a std::string) through the socket.
         *  @param buf the buffer to send
         *  @param length the wanted size to send
         *  @return the size sent through the socket
         */
        virtual int send(const std::string& buf, int length) const = 0;

        /**
         *  Calls the send method with a -1 value for the length
         *  @param buf the buffer to send
         *  @return the instance of the object for chainning purposes
         */
        virtual IClientSocket& operator<<(const std::string& buf) = 0;
};

#endif //__ICLIENT_SOCKET_H__

