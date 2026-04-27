#include"VectorStackImpl.h"
#include<stdexcept>
void VectorStackImpl::push(const ValueType& value)
{
	data.pushBack(value);
}
void VectorStackImpl::pop()
{
	if(isEmpty())
		throw std::logic_error("Stack::pop on empty stack");
	data.popBack();
}
const ValueType& VectorStackImpl::top() const
{
	if(isEmpty())
		throw std::logic_error("Stack::top on empty stack");
	return data.back();
}
bool VectorStackImpl::isEmpty() const
{
	return data.empty();
}
size_t VectorStackImpl::size() const
{
	return data.size();
}
IStackImplementation* VectorStackImpl::clone() const
{
	return new VectorStackImpl(*this);
}
