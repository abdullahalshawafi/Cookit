#pragma once

template < typename T>
class PriorityNode
{
	float priority;
	T item; // A data item
	PriorityNode<T>* next; // Pointer to next node
public:
	PriorityNode();
	PriorityNode(const T& data, const float& pre);
	PriorityNode(const T& data, const float& pre, PriorityNode<T>* nextNodePtr);
	void setItem(const T& data);
	void setNext(PriorityNode<T>* nextNodePtr);
	void setPriority(const float& pre);
	T getItem() const;
	PriorityNode<T>* getNext() const;
	float getPriority();
}; // end Node

template < typename T>
PriorityNode<T>::PriorityNode()
{
	next = nullptr;
}

template < typename T>
PriorityNode<T>::PriorityNode(const T& data, const float& pre)
{
	item = data;
	priority = pre;
	next = nullptr;
}

template < typename T>
PriorityNode<T>::PriorityNode(const T& data, const float& pre, PriorityNode<T>* nextNodePtr)
{
	item = data;
	priority = pre;
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
void PriorityNode<T>::setPriority(const float& pre)
{
	priority = pre;
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

template < typename T>
float PriorityNode<T>::getPriority()
{
	return priority;
}