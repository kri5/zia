#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__

#ifdef WIN32
 #include "Mutex_win32.h"
#else
 #include "Mutex_unix.h"
#endif

template <typename T>
/// Templated Singleton Class Thread Safe
class Singleton
{
  // Methods
  protected:
    Singleton(){}
    virtual ~Singleton(){}

  public:
    /// Return a pointer on the Singleton Instance
    /// If doesn't exist, it creates it.
    static  T*  getInstance()
    {
      if(Singleton<T>::_instance == NULL) 
      {
        Singleton<T>::_mutex->lock();
        if(Singleton<T>::_instance == NULL)
        {
          Singleton<T>::_instance = new T();
        }
        Singleton<T>::_mutex->unlock();
      }
      return static_cast<T*>(Singleton<T>::_instance);
    }

    /// Delete the Instance of Singleton, if exist
    static  void  deleteInstance()
    {
      if (Singleton<T>::_singleton != NULL)
      {
        Singleton<T>::_mutex->lock();
        if (Singleton<T>::_instance != NULL)
        {
          delete  Singleton<T>::_instance;
          Singleton<T>::_instance = NULL;
        }
        Singleton<T>::_mutex->unlock();
      }
    }

  // Attributes
  protected:
    static  T*      _instance;
    static  IMutex* _mutex; 
};

template <typename T>
T*      Singleton<T>::_instance = NULL;
template <typename T>
IMutex* Singleton<T>::_mutex = static_cast<IMutex*>(new Mutex());

#endif /* !__SINGLETON_HPP__ */
