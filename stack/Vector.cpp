//#include"IStackImplementation.h"
//#include"VectorStackImpl.h"
#include"Vector.h"
#include<stdexcept>
#include<utility>
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
//Vector() = default;
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
Vector::Vector(const Vector& other)
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
Vector::Vector(Vector&& other) noexcept
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
void Vector::pushBack(const ValueType& value)
{
	if (_size == _capacity)
	{
		size_t new_cap = (_capacity==0) ? 1 : _capacity * _multiplicativeCoef;
		reserve(new_cap);
	}
	_data[_size++] = value;
}
void Vector::popBack()
{
	if(_size==0)
		throw std::out_of_range("Vector::popBack(): empty vector");
	--_size;
}
const ValueType& Vector::back() const
{
	if(_size==0)
		throw std::logic_error("empty vector");
	return _data[_size-1];
}
bool Vector::empty() const
{
	return _size==0;
}
size_t Vector::size() const
{
	return _size;
}

