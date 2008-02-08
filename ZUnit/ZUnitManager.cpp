#include "ZUnitManager.h"
#include <iostream>

ZUnitManager::ZUnitManager()
{
  _out = new ZUnitStdOutput();
}

ZUnitManager::~ZUnitManager()
{

}

void    ZUnitManager::appendSuite(IZUnitSuite* suite)
{
  _suites.push_back(suite);
}

void    ZUnitManager::run()
{
  std::list<IZUnitSuite*>::iterator it = _suites.begin();
  std::list<IZUnitSuite*>::iterator ite = _suites.end();

  for (; it != ite; ++it)
  {
    (*it)->setOutput(_out);
    (*it)->init();
    _out->newCurrentSuite((*it)->getName());

    try
    {
      (*it)->run();
    }
    catch (const char* msg)
    {
      std::cout << "cought an Exception message = "
        << msg << std::endl;
    }
  }
}
