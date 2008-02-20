#ifndef ISOCKET_H__
 #define ISOCKET_H__

class ISocket
{
	public:
		virtual ~ISocket(){}
		virtual void 			close(bool) const = 0;
		virtual const ISocket&	operator>>(fd_set&) const = 0;
        virtual bool            isSet(const fd_set&) const = 0;
};

#endif //ISOCKET_H__
