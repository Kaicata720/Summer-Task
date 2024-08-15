#ifndef _DEFAULTDATASOURCE_HEADER_INCLUDED_
#define _DEFAULTDATASOURCE_HEADER_INCLUDED_
#include "DataSource.h"

template <typename T>

class DefaultDataSource : public DataSource<T>
{
public:

	 T getNextElement() override;
	 T* getNextSeveralElements(size_t count) override;
	 bool hasNextElement() override;
	 bool reset() override;
	 T operator() (void) override;
	 DataSource<T>& operator>>(T& output) override;
	 operator bool() override;
	 DataSource<T>* clone() const override;
};
#endif // !_DEFAULTDATASOURCE_HEADER_INCLUDED_

template<typename T>
inline T DefaultDataSource<T>::getNextElement()
{
	return T();
}

template<typename T>
inline T* DefaultDataSource<T>::getNextSeveralElements(size_t count)
{
	T* elements = new T[count];
	for (size_t i = 0; i < count; i++)
	{
		elements[i] = T();
	}
	return elements;
}

template<typename T>
inline bool DefaultDataSource<T>::hasNextElement()
{
	return true;
}

template<typename T>
inline bool DefaultDataSource<T>::reset()
{
	return true;
}

template<typename T>
inline T DefaultDataSource<T>::operator()(void)
{
	return getNextElement();
}


template<typename T>
inline DataSource<T>& DefaultDataSource<T>::operator>>(T& output)
{
	output = getNextElement();
	return *this;
}

template<typename T>
inline DefaultDataSource<T>::operator bool() 
{
	return true;
}

template<typename T>
inline DataSource<T>* DefaultDataSource<T>::clone() const
{
	return new DefaultDataSource<T>();
}
