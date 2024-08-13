#ifndef _FILEDATASOURCE_HEADER_INCLUDED_
#define _FILEDATASOURCE_HEADER_INCLUDED_
#include "dataSource.h"
#include "fstream"
template <typename T>

class FileDataSource :public DataSource<T>
{

private:

	std::ifstream textFileStream;
	char fileName[256];

public:
	explicit FileDataSource(const char* fileName);
	~FileDataSource();

	 T getNextElement() override;
	 T* getNextSeveralElements(size_t count) override;
	 bool hasNextElement() override;
	 bool reset() override;
	 T operator() (void) override;
	 DataSource<T>& operator>>(T& output) override;
	 operator bool() override;
	 DataSource<T>* clone() const override;
};

#endif // !_FILEDATASOURCE_HEADER_INCLUDED_

template<typename T>
inline FileDataSource<T>::FileDataSource(const char* fileName)
{
	// Copy the file name into the character arrayw
	strcpy_s(this->fileName, sizeof(this->fileName) - 1, fileName);
	this->fileName[sizeof(this->fileName) - 1] = '\0'; // Ensure null-termination

	textFileStream.open(this->fileName, std::ios::in);

	if (!textFileStream.is_open())
	{
		throw std::runtime_error("Unable to open the file!");
	}
}

template<typename T>
inline FileDataSource<T>::~FileDataSource()
{
	if (textFileStream.is_open()) textFileStream.close();
}

template<typename T>
inline T FileDataSource<T>::getNextElement()
{
	T element;
	if (textFileStream >> element)
	{
		return element;
	}
	
	throw std::runtime_error("No more elements in the file!");
}

template<typename T>
inline T* FileDataSource<T>::getNextSeveralElements(size_t count)
{
	T* elements = new T[count];
	size_t readCount = 0;
	
	try
	{
		for (size_t i = 0; i < count; i++)
		{
			if (textFileStream >> elements[i])
			{
				readCount++;
			}
			else break;
		
		}
	
		if (readCount < count)
		{
			T* resizedElements = new T[readCount];
		
			for (size_t i = 0; i < readCount; ++i)
			{
				resizedElements[i] = elements[i];
			}
			delete[] elements;
			elements = resizedElements;
			resizedElements = nullptr;
		}
	}
	catch (...)
	{
		delete[] elements;
		throw;
	}
	
	return elements;
}

template<typename T>
inline bool FileDataSource<T>::hasNextElement()
{
	return textFileStream.peek() != EOF;
}

template<typename T>
inline bool FileDataSource<T>::reset()
{
	if (textFileStream.is_open())
	{
		textFileStream.clear();
		textFileStream.seekg(0);
		return true;
	}
	return false;
	
}

template<typename T>
inline T FileDataSource<T>::operator()(void)
{
	return getNextElement();
}

template<typename T>
inline DataSource<T>& FileDataSource<T>::operator>>(T& output)
{
	output = getNextElement();
	return *this;
}

template<typename T>
inline FileDataSource<T>::operator bool() 
{
	return textFileStream.peek() != EOF;
}

template<typename T>
inline DataSource<T>* FileDataSource<T>::clone() const
{
	return new FileDataSource<T>(fileName);
}
