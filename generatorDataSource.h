#ifndef _GENERATORDATASOURCE_HEADER_INCLUDED_
#define _GENERATORDATASOURCE_HEADER_INCLUDED_
#include "dataSource.h"

template <typename T>
class GeneratorDataSource : public DataSource<T>

{
public:
	GeneratorDataSource(T (*ptrToFuntion)());
	 T getNextElement() override;
	 T* getNextSeveralElements(size_t count) override;
	 bool hasNextElement() override;
	 bool reset() override;
	 T operator() (void) override;
	 DataSource<T>& operator>>(T& output) override;
	 operator bool() override;
	 DataSource<T>* clone() const override;
private:

	T(*ptrToFunction)();
};

#endif // !_GENERATORDATASOURCE_HEADER_INCLUDED_

template<typename T>
inline GeneratorDataSource<T>::GeneratorDataSource(T(*ptrToFuntion)())
{
	this->ptrToFunction = ptrToFuntion;
}

template<typename T>
inline T GeneratorDataSource<T>::getNextElement()
{
	return ptrToFunction();
}

template<typename T>
inline T* GeneratorDataSource<T>::getNextSeveralElements(size_t count)
{
	T* elements = new T[count];
	for (size_t i = 0; i < count; i++)
	{
		elements[i] = getNextElement();
	}

	return elements;
}

template<typename T>
inline bool GeneratorDataSource<T>::hasNextElement()
{
	return true;
}

template<typename T>
inline bool GeneratorDataSource<T>::reset()
{
	return true;
}

template<typename T>
inline T GeneratorDataSource<T>::operator()(void)
{
	return ptrToFunction();
}

template<typename T>
inline DataSource<T>& GeneratorDataSource<T>::operator>>(T& output)
{
	output = getNextElement();
	return *this;
}

template<typename T>
inline GeneratorDataSource<T>::operator bool()
{
	return hasNextElement();
}

template<typename T>
inline DataSource<T>* GeneratorDataSource<T>::clone() const
{
	return new GeneratorDataSource(ptrToFunction);
}
