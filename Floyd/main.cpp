#include<iostream>
#include<utility>
#include<vector>
struct Node{
	double data;
	Node* next;
	Node(double val) : data(val), next(nullptr)
	{}
};

std::pair<Node*, double> search(Node* head)
{
	if(!head) 
		return {nullptr, -1};
	Node* slow = head;
	Node* fast = head;
	bool res = false;

	while(fast&&fast->next)
	{
		slow = slow->next;
		fast = fast->next->next;
		if (slow==fast)
		{
			res = true;
			break;
		}
	}

	if (!res)
		return {nullptr, -1};

	Node* entry = head;
	int index = 0;
	while(entry!=slow)
	{
		entry = entry->next;
		slow = slow->next;
		++index;
	}

	return {entry, index};
}


Node* create(const std::vector<double>& vec, int start)
{
	if (vec.empty())
		return nullptr;
	Node* head = new Node(vec[0]);
	Node* prev = head;
	Node* startNode = nullptr;
	if (start == 0)
		startNode = head;
	for (size_t i = 0; i<vec.size(); i++)
	{
		Node* node = new Node(vec[i]);
		prev->next = node;
		prev = node;
		if (static_cast<int>(i) == start)
			startNode = node;
	}
	if (startNode)
		prev->next = startNode;
	return head;
}

void res(const std::pair<Node*, int>& result)
{
	Node* cycleNode = result.first;
	int index = result.second;

	if (cycleNode)
	{
		std::cout<<"data "<<cycleNode->data<<" index "<<index<<std::endl;
	}
	else
	{
		std::cout<<"No cycle"<<std::endl;
	}

}
int main()
{
	std::vector<double> vec1 = {1.2,2.1,3.24,4.90,5.244};
	Node* head = create(vec1, 2);

	std::vector<double> vec2 = {1.23, 23.21, 32.312, 432.12};
	Node* head2 = create(vec2, -1);

	std::vector<double> vec3 = {};
	Node* head3 = create(vec3, 2);

	auto result1 = search(head);
	auto result2 = search(head2);
	auto result3 = search(head3);
	
	res(result1);
	res(result2);
	res(result3);
	return 0;
}




