#include"List.h"
#include<stdexcept>
List::List() : head(nullptr), tail(nullptr), sz(0)
	{}
List::~List()
{
	while(head)
	{
		Node* temp = head;
		head = head->next;
		delete temp;
	}
}
List::List(const List& other) : head(nullptr), tail(nullptr), sz(0)
{
	Node* current = other.head;
	while(current)
	{
		push_back(current->value);
		current = current->next;
	}
}
List& List::operator=(const List& other)
{
	if(this == &other)
		return *this;
	while(head)
	{
		Node* temp = head;
		head = head->next;
		delete temp;
	}
	head = tail = nullptr;
	sz = 0; 
	Node* current = other.head;
	while(current)
	{
		push_back(current->value);
		current = current->next;
	}
	return *this;
}
void List::push_back(const ValueType& value)
{
	Node* new_node = new Node(value, tail, nullptr);
	if (tail)
		tail->next = new_node;	
	else
		head = new_node;
	tail = new_node;
	++sz;
}
void List::pop_back()
{
	if (tail==nullptr)
		return;
	Node* temp = tail;
	tail = tail->prev;
	if (tail)
		tail->next = nullptr;
	else
		head = nullptr;
	delete temp;
	--sz;
}
const ValueType& List::back() const
{
	if(!tail)
		throw std::logic_error("empty list");
	return tail->value;
}
bool List::empty() const
{
	return sz==0;
}
size_t List::size() const
{
	return sz;
}








































