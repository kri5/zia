#include "ZUnitSuite.hpp"

class Test2 : public ZUnitSuite<Test2>
{
  public:
    virtual void    init()
    {
      setName("A secong test suite");
      setTarget(this);

      appendTest(&Test2::test1, "le test nb 1");
      //appendTest(&Test2::test2, "le test nb 2");
      //appendTest(&Test2::test3, "le test nb 3");
    }

    void            test1()
    {
      bool  a = true;
      bool  b = false;
      int   c = 42;
      int   d = 21;
      
      assertTrue(a, "test de boolean [true]");
      assertFalse(b, "test de boolean [false]");
      assertTrue(c == d * 2, "test de test (c == d * 2)");
    }

    void            test2()
    {
       
    }

    void            test3()
    {
    
    }
};
