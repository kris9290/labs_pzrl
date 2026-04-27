#include"Vector.h"
#include<stdexcept>
#include<algorithm>
Vector::Vector(const ValueType* rawArray, const size_t size, float coef) : _size(size), _capacity(size), _multiplicativeCoef(coef)
{
	if (_capacity>0)
	{
		_data = new ValueType[_capacity];
		for (size_t i = 0; i < _size; i++)
			_data[i] = rawArray[i];
	}
	else
		_data = nullptr;
}
Vector::Vector(const Vector& other) /*:_size(other._size), _capacity(other._size), _multiplicativeCoef(other._multiplicativeCoef)
{
	if (_capacity>0)
	{
		_data = new ValueType[_capacity];
		for (size_t i = 0; i<_size; i++)
			_data[i] = other._data[i];
	}
	else
		_data = nullptr;
}*/
{
	*this = other;
}
Vector& Vector::operator=(const Vector& other)
{
	if (this==&other)
		return *this;
	ValueType* newData = nullptr;
	if(other._size>0)
	{
		newData = new ValueType[other._capacity];
		for (size_t i = 0; i<other._size; i++)
			newData[i] = other._data[i];
	}
	delete[]_data;
	_data = newData;
	_size = other._size;
	_capacity = other._size;
	_multiplicativeCoef = other._multiplicativeCoef;
	return *this;
}
Vector::Vector(Vector&& other) noexcept/*: _data(other._data), _size(other._size), _capacity(other._capacity), _multiplicativeCoef(other._multiplicativeCoef)
{
	other._data = nullptr;
	other._size = 0;
	other._capacity = 0;
	other._multiplicativeCoef = 2.0f;
}*/
{
	*this = std::move(other);
}
Vector& Vector::operator=(Vector&& other) noexcept
{
	if (this==&other)
		return *this;
	delete[]_data;
       	_data = other._data;
       	_size = other._size;
	_capacity = other._capacity;
	_multiplicativeCoef = other._multiplicativeCoef;
	other._data = nullptr;
	other._size = 0;
	other._capacity = 0;
	other._multiplicativeCoef = 2.0f;
	return *this;
}
Vector::~Vector()
{
	delete[]_data;
}
//-----------------------------------------

void Vector::ensureCapacity(size_t minCapacity)
{
	if (minCapacity<=_capacity)
		return;
	size_t newCap = _capacity;
	if (newCap==0)
		newCap = std::max<size_t>(1, static_cast<size_t>(_multiplicativeCoef));
	while (newCap<minCapacity)
	{
		newCap = static_cast<size_t>(newCap*_multiplicativeCoef);
	}
	ValueType* newData = new ValueType[newCap];
	for (size_t i = 0; i<_size; ++i)
		newData[i] = _data[i];
	delete[]_data;
	_data = newData;
	_capacity = newCap;
}


//-------------------------------------

void Vector::pushBack(const ValueType& value)
{
	ensureCapacity(_size+1);
	_data[_size++] = value;
}

void Vector::pushFront(const ValueType& value)
{
	ensureCapacity(_size+1);
	for (size_t i = _size; i>0; --i)
		_data[i] = _data[i-1];
	_data[0] = value;
	_size+=1;
}

void Vector::insert(const ValueType& value, size_t pos)
{
	if (pos>_size)
		return;
	ensureCapacity(_size+1);
	for (size_t i = _size; i>pos; --i)
		_data[i] = _data[i-1];
	_data[pos] = value;
	_size+=1;
}

void Vector::insert(const ValueType* values, size_t size, size_t pos)
{
	if (size==0 || pos>_size)
		return;
	ensureCapacity(_size+size);
	for(size_t i = _size; i>pos; --i)
	{
		_data[i+size-1] = _data[i-1];
	}
	for(size_t i = 0; i<size; ++i)
	{
		_data[pos+i] = values[i];
	}
	_size+=size;
}

void Vector::insert(const Vector& vector, size_t pos)
{
	insert(vector._data, vector._size, pos);
}

void Vector::popBack()
{
	if(_size==0)
		throw std::out_of_range("Vector::popBack(): empty vector");
	--_size;
}


void Vector::popFront()
{
	if (_size==0)
		throw std::out_of_range("Vector::popFront(): empty vector");
	for (size_t i = 0; i<_size-1; i++)
		_data[i] = _data[i+1];
	_size-=1;
}

void Vector::erase(size_t pos, size_t count)
{
	if(pos>=_size || count == 0)
		return;
	if(pos+count>_size)
		count = _size-pos;
	for(size_t i = pos; i+count<_size; ++i)
		_data[i] = _data[i+count];
	_size -= count;
}
void Vector::eraseBetween(size_t beginPos, size_t endPos)
{
	if (beginPos>=_size || beginPos>=endPos)
		return;
	if(endPos>_size)
		endPos = _size;
	erase(beginPos, endPos-beginPos);
}

size_t Vector::size() const
{
	return _size;
}

size_t Vector::capacity() const
{
	return _capacity;
}

double Vector::loadFactor() const
{
	if (_capacity == 0)
		return 0.0;
	return static_cast<double>(_size)/static_cast<double>(_capacity);
}

ValueType& Vector::operator[](size_t idx)
{
	return _data[idx];
}
const ValueType& Vector::operator[](size_t idx) const
{
	return _data[idx];
}
long long Vector::find(const ValueType& value) const
{
	for (size_t i = 0; i<_size; i++)
	{
		if (_data[i]==value)
			return static_cast<long long>(i);
	}
	return -1;
}
void Vector::reserve(size_t capacity)
{
	if (capacity<=_capacity)
		return;
	if(capacity == 0)
	{
		delete[]_data;
		_data = nullptr;
		_capacity = 0;
		return;
	}
	ValueType* newData = new ValueType[capacity];
	for (size_t i = 0; i<_size; ++i)
		newData[i] = _data[i];
	delete[]_data;
	_data = newData;
	_capacity = capacity;
}
void Vector::shrinkToFit()
{
	if(_size == _capacity)
		return;
	if (_size == 0)
	{
		delete[]_data;
		_data = nullptr;
		_capacity = 0;
		return;
	}
	ValueType* newData = new ValueType[_size];
	for (size_t i = 0; i<_size; i++)
		newData[i] = _data[i];
	delete[]_data;
	_data = newData;
	_capacity = _size;
}

Vector::Iterator::Iterator(ValueType* ptr) : _ptr(ptr)
	{}
ValueType& Vector::Iterator::operator*()
{
	return *_ptr;
}
const ValueType& Vector::Iterator::operator*() const
{
	return *_ptr;
}

ValueType* Vector::Iterator::operator->()
{
	return _ptr;
}
const ValueType* Vector::Iterator::operator->() const
{
	return _ptr;
}
Vector::Iterator Vector::Iterator::operator++()
{
	_ptr+=1;
	return *this;
}
Vector::Iterator Vector::Iterator::operator++(int)
{
	Iterator pt = *this;
	_ptr+=1;
	return pt;
}
bool Vector::Iterator::operator==(const Iterator& other) const
{
	return _ptr==other._ptr;
}
bool Vector::Iterator::operator!=(const Iterator& other) const
{
	return _ptr!=other._ptr;
}
Vector::Iterator Vector::begin()
{
	return Iterator(_data);
}
Vector::Iterator Vector::end()
{
	return Iterator(_data+_size);
}































