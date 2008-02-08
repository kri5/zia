#ifndef __ZUNITTEST_H__
# define __ZUNITTEST_H__

#include "IZUnitSuite.h"
#include "ZUnitOutput.h"

#include <list>
#include <map>
#include <string>
#include <iostream>

template <class T>
class ZUnitSuite : public IZUnitSuite
{
  public:
    typedef void (T::*TestMethod)();

    struct Test
    {
      Test(const std::string& _name, 
          TestMethod _method) : name(_name), method(_method){}
      std::string   name;
      TestMethod    method;
    };

    ZUnitSuite(const std::string& = "NoName");
    virtual ~ZUnitSuite(){}

    void                run();
    void                setName(const std::string& name);
    const std::string&  getName() const;
    void                setOutput(ZUnitOutput*);
    

  protected:
    void    appendTest(TestMethod, const std::string& = "");
    void    setTarget(T*);

  private:
    std::string             _name;
    std::list<Test>         _tests;
    T*                      _target;
    ZUnitOutput*            _out;

  public:
    /**
     *  Checks equality of the two values
     *
     *  @param a the first value
     *  @param b the second value
     *  @param name the name of the assert
     * */
    template<typename U>
    void          assertEqual(U a, U b, const std::string& name)
    {
      _out->assertResult(a == b, name);
    }

    /**
     *  Checks unequality of the two values
     *
     *  @param a first value
     *  @param b second value
     *  @param name the name of the assert
     * */
    template<typename U>
    void        assertNotEqual(U a, U b, const std::string& name)
    {
      _out->assertResult(a != b, name);
    }

    /**
     *  Checks if the value is NULL
     *
     *  @param a the value to check
     *  @param name the name of the assert
     * */
    template<typename U>
    void        assertNull(U a, const std::string& name)
    {
      _out->assertResult(a == NULL, name);
    }

    /**
     *  Checks if the value is not NULL
     *
     *  @param a the value to check
     *  @param name the name of the assert
     * */
    template<typename U>
    void        assertNotNull(U a, const std::string& name)
    {
      _out->assertResult(a != NULL, name);
    }

    /**
     *  Checks if the value is true 
     *
     *  @param a the value to check
     *  @param name the name of the assert
     * */
    void        assertTrue(bool a, const std::string& name)
    {
      _out->assertResult(a, name);
    }

    /**
     *  Checks if the value false 
     *
     *  @param a the value to check
     *  @param name the name of the assert
     * */
    void        assertFalse(bool a, const std::string& name)
    {
      _out->assertResult(!a, name);
    }
};


/**
 *  ZUnitSuite constructor, takes the test's name
 *  as a std::string in parameter. The name olso
 *  have a default value so it's not required, but
 *  still recommended for the results understanding
 *
 *  @param name the name of the suite
 * */
template<class T>
ZUnitSuite<T>::ZUnitSuite(const std::string& name)
{
  _name = name;
  _out = 0;
  _target = 0;
}

/**
 *  Set ZUnitSuite's name
 *
 *  @param name the name of the suite
 * */
template<class T>
void    ZUnitSuite<T>::setName(const std::string& name)
{
  _name = name;
}

/**
 *  This method returns ZUnitSuite's name
 *  as a const std::string reference
 *
 *  @return the suite name
 * */
template<class T>
const std::string&    ZUnitSuite<T>::getName() const
{
  return _name;
}

/**
 *  Append a test method to the ZUnitSuite list.
 *  The appened method will be called in apropriated time
 * */
template<class T>
void    ZUnitSuite<T>::appendTest(TestMethod method, 
                                  const std::string& name)
{
  _tests.push_back(Test(name, method));
}

/**
 *  This method runs all the tests appended by it's
 *  child in his init(). Before each test, it calls
 *  the newCurrentTest method of the current output 
 *  class to set the name of the test.
 * */
template<class T>
void    ZUnitSuite<T>::run()
{
  // FIXME: create an Exception class
  // to handle the error properly
  if (!_out)    throw "No output";
  if (!_target) throw "No target";

  typename std::list<Test>::iterator  it = _tests.begin();
  typename std::list<Test>::iterator  ite = _tests.end();

  for (; it != ite; ++it)
  {
    _out->newCurrentTest((*it).name);
    (_target->*(*it).method)();
  }
}

/**
 *  Sets the output class which manages
 *  the output results
 *
 *  @param out the output class
 * */
template<class T>
void    ZUnitSuite<T>::setOutput(ZUnitOutput* out)
{
  _out = out;
}

/**
 *  Sets the target class which extends ZUnitSuite.
 *  This method must be called in the init() method
 *  of each child class or else, an exception will be
 *  thrown later.
 *
 *  @param target the class target instance
 * */
template<class T>
void    ZUnitSuite<T>::setTarget(T* target)
{
  _target = target;
}

#endif /* !__ZUNITTEST_H__ */
