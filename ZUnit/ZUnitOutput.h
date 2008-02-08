#ifndef __ZUNITOUTPUT_H__
# define __ZUNITOUTPUT_H__

#include <string>

class ZUnitOutput
{
  public:
    virtual       ~ZUnitOutput(){}
    virtual void  newCurrentSuite(const std::string&) = 0;
    virtual void  newCurrentTest(const std::string&) = 0;
    virtual void  assertResult(bool, const std::string&) = 0;
};

#endif /* !__ZUNITOUTPUT_H__ */
