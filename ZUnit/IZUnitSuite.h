#ifndef __IZUNITTEST_H__
# define __IZUNITTEST_H__

#include <string>
#include "ZUnitOutput.h"

class IZUnitSuite
{
  public:
    virtual ~IZUnitSuite(){}
    virtual void                init() = 0;
    virtual void                run() = 0;
    virtual const std::string&  getName() const = 0;
    virtual void                setOutput(ZUnitOutput*) = 0;
};

#endif /* !__IZUNITTEST_H__ */
