#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__

#ifdef WIN32
 #include "Mutex_win32.h"
#else
 #include "Mutex_unix.h"
#endif

template <typename T>
class Singleton
{
  // Methods
  protected:
    Singleton(){}
    virtual ~Singleton(){}

  public:
    static  T*  getInstance()
    {
      if(Singleton<T>::_instance == NULL) 
      {
        Singleton<T>::_mutex->lock();
        if(Singleton<T>::_instance == NULL)
        {
          Singleton<T>::_instance = new T();
          _count++;
        }
        Singleton<T>::_mutex->unlock();
      }
      return static_cast<T*>(Singleton<T>::_instance);
    }

    static  void  deleteInstance()
    {
      if (Singleton<T>::_singleton != NULL)
      {
        Singleton<T>::_mutex->lock();
        if (Singleton<T>::_instance != NULL)
        {
          delete  Singleton<T>::_instance;
          _count--;
          Singleton<T>::_instance = NULL;
        }
        Singleton<T>::_mutex->unlock();
      }
    }

  // Attributes
  protected:
    static  T*      _instance;
    static  IMutex* _mutex; 
  public:
    static  int     _count;
};

template <typename T>
T*      Singleton<T>::_instance = NULL;
template <typename T>
IMutex* Singleton<T>::_mutex = static_cast<IMutex*>(new Mutex());
template <typename T>
int     Singleton<T>::_count = 0;

#endif /* !__SINGLETON_HPP__ */
