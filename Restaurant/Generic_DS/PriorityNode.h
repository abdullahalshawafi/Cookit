#pragma once

template < typename T>
class PriorityNode
{
	int priority;
	T item; // A data item
	PriorityNode<T>* next; // Pointer to next node
public:
	PriorityNode();
	PriorityNode(const T& data, int pre);
	PriorityNode(const T& data, PriorityNode<T>* nextNodePtr);
	int GetPriority();
	void setItem(const T& data);
	void setNext(PriorityNode<T>* nextNodePtr);
	T getItem() const;
	PriorityNode<T>* getNext() const;
}; // end Node

template < typename T>
int PriorityNode<T>::GetPriority()
{
	return priority;
}

template < typename T>
PriorityNode<T>::PriorityNode()
{
	next = nullptr;
}

template < typename T>
PriorityNode<T>::PriorityNode(const T& data, int pre)
{
	priority = pre;
	item = data;
	next = nullptr;
}

template < typename T>
PriorityNode<T>::PriorityNode(const T& data, PriorityNode<T>* nextNodePtr)
{
	item = data;
	next = nextNodePtr;
}

template < typename T>
void PriorityNode<T>::setItem(const T& data)
{
	item = data;
}

template < typename T>
void PriorityNode<T>::setNext(PriorityNode<T>* nextNodePtr)
{
	next = nextNodePtr;
}

template < typename T>
T PriorityNode<T>::getItem() const
{
	return item;
}

template < typename T>
PriorityNode<T>* PriorityNode<T>::getNext() const
{
	return next;
}

