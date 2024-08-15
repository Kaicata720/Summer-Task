#ifndef _DATASOURCE_HEADER_INCLUDED_
#define _DATASOURCE_HEADER_INCLUDED_

template <typename T>

class DataSource
{
public:
	virtual T getNextElement() = 0;
	virtual T* getNextSeveralElements(size_t count) = 0;
	virtual bool hasNextElement() = 0;
	virtual bool reset() = 0;
	virtual T operator() (void) = 0;
	virtual DataSource<T>& operator>>(T& output) = 0;
	virtual operator bool() = 0;
	
	virtual DataSource<T>* clone() const = 0;
	virtual ~DataSource() = default;

};
#endif // !_DATASOURCE_HEADER_INCLUDED_

