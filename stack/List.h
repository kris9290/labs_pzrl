#pragma once
#include"Stack.h"
#include"IStackImplementation.h"
#include<cstddef>
class List{
private:
	struct Node{
		ValueType value;
		Node* prev;
		Node* next;
		Node(const ValueType& v, Node* p = nullptr, Node* n = nullptr) : value(v), prev(p), next(n)
		{}
	};
	Node* head;
	Node* tail;
	size_t sz;
public:
	List();
	~List();
	List(const List& other);
	List& operator=(const List& other);
	void push_back(const ValueType& value);
	void pop_back();
	const ValueType& back() const;
	bool empty() const;
	size_t size() const;
};

