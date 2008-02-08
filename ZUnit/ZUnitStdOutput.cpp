#include "ZUnitStdOutput.h"

ZUnitStdOutput::ZUnitStdOutput()
{
  _suiteNb = 0;
  _testNb = 0;
  _assertNb = 0;
}

ZUnitStdOutput::~ZUnitStdOutput()
{

}

void    ZUnitStdOutput::newCurrentSuite(const std::string& name)
{
  if (_suiteNb)
    std::cout << "|========" << std::endl << "|" << std::endl;
  std::cout << "|======== Suite [" << name << "]" << std::endl;
  _suiteNb++;
  _testNb = 0;
  _assertNb = 0;
}

void    ZUnitStdOutput::newCurrentTest(const std::string& name)
{
  if (_testNb)
    std::cout << "|===" << std::endl << "|" << std::endl;
  std::cout << "|==== Test [" << name << "]" << std::endl;
  _testNb++;
}

void    ZUnitStdOutput::assertResult(bool success, const std::string& name)
{
  std::cout << "|== assert [" << name << "]" << " "
    << (success ? "succeeded" : "failed") << std::endl;
}
