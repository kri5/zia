#include "Test1.cpp"
#include "Test2.cpp"
#include "ZUnitManager.h"

int     main()
{
  ZUnitManager*  manager = new ZUnitManager();

  Test1* test1 = new Test1();
  Test2* test2 = new Test2();

  manager->appendSuite(test1);
  manager->appendSuite(test2);

  manager->run();
  return 0;
}
