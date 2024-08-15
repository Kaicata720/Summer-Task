#ifndef _ARRAYDATASOURCE_HEADER_INCLUDED_
#define _ARRAYDATASOURCE_HEADER_INCLUDED_
#include "DataSource.h"
template <typename T>

class ArrayDataSource : public DataSource<T>
{
public:
	ArrayDataSource() : array(nullptr), size(0), position(0) {}
	ArrayDataSource(const T* array, size_t size);
	ArrayDataSource(const ArrayDataSource<T>& other);
	ArrayDataSource<T>& operator=(const ArrayDataSource<T>& other);
	~ArrayDataSource();
	

	 T getNextElement() override;
	 T* getNextSeveralElements(size_t count) override;
	 bool hasNextElement() override;
	 bool reset() override;
	 T operator()(void) override;
	 DataSource<T>& operator>>(T& output) override;
	 operator bool() override;
		
	 DataSource<T>* clone() const override;

	 ArrayDataSource<T>& operator+(const T& newElement);
	 ArrayDataSource<T>& operator+=(const T& newElement);
	 ArrayDataSource<T>& operator--();
	 ArrayDataSource<T>& operator--(int);



private:
	T* array;
	size_t size;
	size_t position;
};

#endif // !_ARRAYDATASOURCE_HEADER_INCLUDED_

template<typename T>
inline ArrayDataSource<T>::ArrayDataSource(const T* array, size_t size)
{
	if (array == nullptr || size == 0)
	{
		throw std::invalid_argument("The elements array cannot be empty or null!");
	}
	this->array = new T[size];
	this->size = size;
	this->position = 0;
	for (int i = 0; i < size; i++)
	{
		this->array[i] = array[i];
	}
}

template<typename T>
inline ArrayDataSource<T>::ArrayDataSource(const ArrayDataSource<T>& other)
{
	array = new T[other.size];
	size = other.size;
	this->position = 0;
	for (size_t i = 0; i < other.size; i++)
	{
		array[i] = other.array[i];
	}
	
}

template<typename T>
inline ArrayDataSource<T>& ArrayDataSource<T>::operator=(const ArrayDataSource<T>& other)
{
	if (other.size == 0)
	{
		throw std::runtime_error("Attempting to assign ArrayDataSource with size 0 of it's array!");
	}

	if (this != &other) 
	{
		T* newArray = new T[other.size];

		for (size_t i = 0; i < other.size; i++)
		{
			newArray[i] = other.array[i];
		}

		delete[] array;
		array = newArray;
		newArray = nullptr;
		size = other.size;
		position = 0;
		
	}
	return *this;
}

template<typename T>
inline ArrayDataSource<T>::~ArrayDataSource()
{
	if (array) delete[] array;
}

template<typename T>
inline T ArrayDataSource<T>::getNextElement()
{
	if (hasNextElement())
	{
		++position;
		return array[position - 1];
	}
	throw std::runtime_error("There are no more elements in the array!");
}

template<typename T>
inline T* ArrayDataSource<T>::getNextSeveralElements(size_t count)
{
	if (count == 0)
	{
		throw std::runtime_error("Can't get 0 elements!");
	}
	
	size_t readCount = 0;
	if (position + count > size)
	{
		readCount = size - position;
	}
	else
	{
		readCount = count;
	}

	T* elements = new T[readCount];
	for (size_t i = 0; i < readCount; i++)
	{
		elements[i] = getNextElement();
	}
	return elements;
}

template<typename T>
inline bool ArrayDataSource<T>::hasNextElement()
{
	if (position < size)
	{
		return true;
	}
	return false;
}

template<typename T>
inline bool ArrayDataSource<T>::reset()
{
	position = 0;
	return true;
}

template<typename T>
inline T ArrayDataSource<T>::operator()(void)
{
	return  getNextElement();
}


template<typename T>
inline DataSource<T>& ArrayDataSource<T>::operator>>(T& output)
{
	output = getNextElement();
	return *this;
}

template<typename T>
inline ArrayDataSource<T>::operator bool()
{
	return hasNextElement();
}

template<typename T>
inline DataSource<T>* ArrayDataSource<T>::clone() const
{
	return new ArrayDataSource<T>(*this);
}

template<typename T>
inline ArrayDataSource<T>& ArrayDataSource<T>::operator+(const T& newElement) 
{
	*this += newElement;
	return *this;
}

template<typename T>
inline ArrayDataSource<T>& ArrayDataSource<T>::operator+=(const T& newElement) 
{
	T* newArray = new T[size + 1];
	for (size_t i = 0; i < size; ++i) {
		newArray[i] = array[i];
	}
	newArray[size] = newElement;

	delete[] array;
	array = newArray;
	++size;

	return *this;
}

template<typename T>
inline ArrayDataSource<T>& ArrayDataSource<T>::operator--()
{
	if (position > 0)
	{
		--position;
	}
	return *this;
}

template<typename T>
inline ArrayDataSource<T>& ArrayDataSource<T>::operator--(int)
{
	-*this;
	return *this;
}
