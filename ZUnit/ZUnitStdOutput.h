#ifndef __ZUNITSTDOUTPUT_H__
# define __ZUNITSTDOUPUT_H__

#include <string>
#include <iostream>

#include "ZUnitOutput.h"

class ZUnitStdOutput : public ZUnitOutput
{
  public:
            ZUnitStdOutput();
    virtual ~ZUnitStdOutput();

    void    newCurrentSuite(const std::string&);
    void    newCurrentTest(const std::string&);
    void    assertResult(bool, const std::string&);

  private:
    int     _suiteNb; 
    int     _testNb;
    int     _assertNb;
};

#endif /* !__ZUNITSTDOUTPUT_H__ */
