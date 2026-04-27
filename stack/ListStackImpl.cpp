#include"ListStackImpl.h"
#include<stdexcept>
void ListStackImpl::push(const ValueType& value)
{
	data.push_back(value);
}
void ListStackImpl::pop()
{
	if(isEmpty())
		throw std::logic_error("Stack::pop on empty stack");
	data.pop_back();
}
const ValueType& ListStackImpl::top() const
{
	if(isEmpty())
		throw std::logic_error("Stack::top on empty stack");
	return data.back();
}
bool ListStackImpl::isEmpty() const
{
	return data.empty();
}
size_t ListStackImpl::size() const
{
	return data.size();
}
IStackImplementation* ListStackImpl::clone() const
{
	return new ListStackImpl(*this);
}
