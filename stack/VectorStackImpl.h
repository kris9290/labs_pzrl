#pragma once
#include"IStackImplementation.h"
#include"Vector.h"
class VectorStackImpl : public IStackImplementation
{
private:
	Vector data;
public:
	void push(const ValueType& value) override;
	void pop() override;
	const ValueType& top() const override;
	bool isEmpty() const override;
	size_t size() const override;
	IStackImplementation* clone() const override;
	virtual ~VectorStackImpl() = default;
};
