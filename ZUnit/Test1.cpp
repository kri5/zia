#include "ZUnitSuite.hpp"
#include <iostream>

class Test1 : public ZUnitSuite<Test1>
{
  public:
    Test1()
    {
    }

    virtual void    init()
    {
      setName("A stupid Test Suite");
      setTarget(this);
      appendTest(&Test1::test1, "le premier test");
      appendTest(&Test1::test2, "et le deusieme");
    }

    void            test1()
    {
      double a = 342;
      double b = 424;

      assertEqual(1, 3, "1 == 3");
      assertEqual(a, b, "a == b (double)");
    }

    void            test2()
    {
      assertEqual(2, 2, "2 == 2");
      assertEqual("test", "test", "test == test");
    }
};
