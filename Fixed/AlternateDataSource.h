#ifndef _ALTERNATEDATASOURCE_HEADER_INCLUDED_
#define _ALTERNATEDATASOURCE_HEADER_INCLUDED_
#include "DataSource.h"

template <typename T>

class AlternateDataSource : public DataSource<T>
{
public:
	AlternateDataSource(): dataSources(nullptr),size(0),position(0) {}
	AlternateDataSource(const AlternateDataSource& other);
	AlternateDataSource<T>& operator=(const AlternateDataSource& other);
	AlternateDataSource(const DataSource<T>** dataSources,size_t size);
	~AlternateDataSource();


	 T getNextElement() override;
	 T* getNextSeveralElements(size_t count) override;
	 bool hasNextElement() override;
	 bool reset() override;
	 T operator() (void) override;
	 DataSource<T>& operator>>(T& output) override;
	 operator bool() override;
	 DataSource<T>* clone() const override;
private:
	DataSource<T>** dataSources;
	size_t size;
	size_t position;

	void clearMemory();

};
#endif // !_ALTERNATEDATASOURCE_HEADER_INCLUDED_

template<typename T>
inline AlternateDataSource<T>::AlternateDataSource(const AlternateDataSource& other)
{
	if (other.size == 0 || other.dataSources == nullptr)
	{
		throw std::invalid_argument("The sources array cannot be empty or null.");
	}
	dataSources = new DataSource<T>*[other.size];
	for (size_t i = 0; i < other.size; i++)
	{
		if (other.dataSources[i] == nullptr)
		{
			throw std::invalid_argument("One or more sources are null.");
		}
		dataSources[i] = other.dataSources[i]->clone();
	}
	this->size = other.size;
	position = 0;
}

template<typename T>
inline AlternateDataSource<T>& AlternateDataSource<T>::operator=(const AlternateDataSource& other)
{
	if (this == &other)
	{
		// Handle self-assignment
		return *this;
	}

	DataSource<T>** newDataSources = nullptr;

	try
	{
		if (other.size == 0 || other.dataSources == nullptr)
		{
			throw std::invalid_argument("The sources array cannot be empty or null.");
		}

		newDataSources = new DataSource<T>*[other.size];
		for (size_t i = 0; i < other.size; i++)
		{
			if (other.dataSources[i] == nullptr)
			{
				throw std::invalid_argument("One or more sources are null.");
			}

			newDataSources[i] = other.dataSources[i]->clone();
		}

		clearMemory();
		this->dataSources = newDataSources;
		this->size = other.size;
		position = 0;
		newDataSources = nullptr;
	}
	catch (...)
	{
		// Clean up temporary resources if an exception is thrown
		for (size_t i = 0; i < other.size; ++i)
		{
			delete newDataSources[i];
		}
		delete[] newDataSources;

		// Rethrow the exception
		throw;
	}
	
	return *this;
}

template<typename T>
inline AlternateDataSource<T>::AlternateDataSource(const DataSource<T>** dataSources, size_t size)
{
	if (size == 0 || dataSources == nullptr)
	{
		throw std::invalid_argument("The sources array cannot be empty or null.");
	}
	this->dataSources = new DataSource<T>*[size];

	for (size_t i = 0; i < size; i++)
	{
		if (dataSources[i] == nullptr)
		{
			this->size = i; 
			clearMemory();
			throw std::invalid_argument("One or more sources are null.");
		}
		this->dataSources[i] = dataSources[i]->clone();
	}
	this->size = size;
	position = 0;
}

template<typename T>
inline AlternateDataSource<T>::~AlternateDataSource()
{
	clearMemory();
}

template<typename T>
inline void AlternateDataSource<T>::clearMemory()
{
	for (size_t i = 0; i < size; ++i)
	{
		delete dataSources[i];
	}
	delete[] dataSources;
}

template<typename T>
inline T AlternateDataSource<T>::getNextElement()
{
	for (size_t i = 0; i < size; ++i)
	{
		if (dataSources[position]->hasNextElement())
		{
			T element = dataSources[position]->getNextElement();
			position = (position + 1) % size; // Move to the next source
			return element;
		}

		position = (position + 1) % size; // Move to the next source
	}

	throw std::runtime_error("All sources are exhausted");
}

template<typename T>
inline T* AlternateDataSource<T>::getNextSeveralElements(size_t count)
{
	T* elements = new T[count];
	size_t collected = 0;

	while (collected < count)
	{
		if (!hasNextElement())
		{
			break;
		}

		elements[collected++] = getNextElement();
	}

	if (collected < count)
	{
		// Resize the array if fewer elements were collected
		T* resizedArray = new T[collected];
		for (size_t i = 0; i < collected; ++i)
		{
			resizedArray[i] = elements[i];
		}

		delete[] elements;
		return resizedArray;
	}

	return elements;
}

template<typename T>
inline bool AlternateDataSource<T>::hasNextElement()
{
	for (size_t i = 0; i < size; ++i)
	{
		if (dataSources[i]->hasNextElement())
		{
			return true;
		}
	}

	return false;
}

template<typename T>
inline bool AlternateDataSource<T>::reset()
{
	bool resetSuccessful = true;

	for (size_t i = 0; i < size; ++i)
	{
		if (!dataSources[i]->reset())
		{
			resetSuccessful = false;
		}
	}

	position = 0;
	return resetSuccessful;
}

template<typename T>
inline T AlternateDataSource<T>::operator()(void)
{
	return getNextElement();
}

template<typename T>
inline DataSource<T>& AlternateDataSource<T>::operator>>(T& output)
{
	output = getNextElement();
	return *this;
}

template<typename T>
inline AlternateDataSource<T>::operator bool()
{
	return hasNextElement();
}

template<typename T>
inline DataSource<T>* AlternateDataSource<T>::clone() const
{
	return new AlternateDataSource<T>(*this);
}
