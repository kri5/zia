#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__

template <typename T>
class Singleton
{
public:
	static T*		getInstance()
	{
		if (Singleton<T>::_singleton)
			return (Singleton<T>::_singleton);
		Singleton<T>::_singleton = new T();
		return (Singleton<T>::_singleton);
	}
	static void     deleteInstance()
	{
		if (Singleton<T>::_singleton)
		{
			delete Singleton<T>::_singleton;
			Singleton<T>::_singleton = NULL;
		}
	}
protected:
	static T*		_singleton;
	Singleton(){}
	virtual ~Singleton(){}
};

template <typename T>
T*  Singleton<T>::_singleton = NULL;

#endif // __SINGLETON_HPP__