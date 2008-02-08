#ifndef __ZUNITMANAGER_H__
# define __ZUNITMANAGER_H__

#include "IZUnitSuite.h"
#include "ZUnitStdOutput.h"
#include "ZUnitOutput.h"
#include <list>

class ZUnitManager
{
  public:
    ZUnitManager();
    ~ZUnitManager();

    void    appendSuite(IZUnitSuite*);
    void    run();

  private:
    std::list<IZUnitSuite*>     _suites;
    ZUnitOutput*                _out;
};

#endif /* !__ZUNITMANAGER_H__ */
