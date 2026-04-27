#pragma once
#include"List.h"
#include"IStackImplementation.h"
class ListStackImpl : public IStackImplementation
{
private:
	List data;
public:
	void push(const ValueType& value) override;
	void pop() override;
	const ValueType& top() const override;
	bool isEmpty() const override;
	size_t size() const override;
	IStackImplementation* clone() const override;
	virtual ~ListStackImpl() = default;
};
