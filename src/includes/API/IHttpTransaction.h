#ifndef __IHTTPTRANSACTION_H__
# define __IHTTPTRANSACTION_H__

class IHttpTransaction
{
    public:
        virtual ~IHttpTransaction() {};

        virtual void                                        setOption(std::string, std::string&) = 0;
        virtual void                                        setOptions(std::map<std::string, std::string>) = 0;
        virtual std::string                                 getOption(std::string) const = 0;
        virtual const std::map<std::string, std::string>&   getOpions() const = 0;

        virtual void                                        setProtocol(std::string&) = 0;
        virtual std::string&                                getProtocol() const = 0;
};

# endif /* !__IHTTPTRANSACTION_H__ */
