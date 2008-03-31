#ifndef ICONFIG_H__
# define ICONFIG_H__

class   IConfig
{
    public:
        virtual ~IConfig(){}

        virtual const std::string&                  getParam(const std::string&) const = 0;
        virtual void                                setParam(const std::string&, const std::string&) = 0;
        virtual bool                                isSet(const std::string&) const = 0;
};

#endif //ICONFIG_H__

