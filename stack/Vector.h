#pragma once
#include"Stack.h"
#include<cstddef>
class Vector{
private:
	ValueType* _data = nullptr;
	size_t _size = 0;
	size_t _capacity = 0;
	float _multiplicativeCoef = 2.0f;
public:
	void reserve(size_t capacity);
	Vector() = default;
	Vector(const ValueType* rawArray, const size_t size, float coef);
	explicit Vector(const Vector& other);
	Vector& operator=(const Vector& other);
	Vector(Vector&& other) noexcept;
	Vector& operator=(Vector&& other) noexcept;
	~Vector();
	void pushBack(const ValueType& value);
	void popBack();
	const ValueType& back() const;
	bool empty() const;
	size_t size() const;
};
