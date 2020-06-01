#pragma once
#include "PriorityNode.h"

template < typename T>
class PriorityQueue
{
	PriorityNode<T>* backPtr;
	PriorityNode<T>* frontPtr;
public:
	PriorityQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry, float Per);
	bool dequeue(T& frntEntry);
	bool peekFront(T& frntEntry)  const;
	void DeleteNode(const int& item);
	T* toArray(int& count);
	PriorityNode<T>* SearchForOrder(T Ord);
	~PriorityQueue();
};

/*
Function: PriorityQueue()
The constructor of the PriorityQueue class.
*/
template < typename T>
PriorityQueue<T>::PriorityQueue<T>()
{
	backPtr = nullptr;
	frontPtr = nullptr;
}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: isEmpty
Sees whether this queue is empty.

Input: None.
Output: True if the queue is empty; otherwise false.
*/
template < typename T>
bool PriorityQueue<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}
/////////////////////////////////////////////////////////////////////////////////////////

/*Function:enqueue
Adds newEntry at the back of this queue.

Input: newEntry .
Output: True if the operation is successful; otherwise false.
*/
template < typename T>
bool PriorityQueue<T>::enqueue(const T& newEntry, float pre)
{
	PriorityNode<T>* temp = frontPtr;
	PriorityNode<T>* newNodePtr = new PriorityNode<T>(newEntry, pre);
	// Insert the new node
	if (isEmpty())// The queue is empty
	{
		frontPtr = newNodePtr;
		backPtr = newNodePtr;
		return true;
	}

	if (newNodePtr->getPriority() > temp->getPriority() || newNodePtr->getItem()->GetUrgent())
	{
		frontPtr = newNodePtr;
		newNodePtr->setNext(temp);
		return true;
	}

	while (temp->getNext() && temp->getNext()->getPriority() >= newNodePtr->getPriority())
		temp = temp->getNext();

	newNodePtr->setNext(temp->getNext());
	temp->setNext(newNodePtr);

	if (!newNodePtr->getNext())
		backPtr = newNodePtr;
	return true;
} // end enqueue
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Function: dequeue
Removes the front of this queue. That is, removes the item that was added
earliest.

Input: None.
Output: True if the operation is successful; otherwise false.
*/
template < typename T>
bool PriorityQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	PriorityNode<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: one node in queue
		backPtr = nullptr;

	// Free memory reserved by the dequeued node
	delete nodeToDeletePtr;
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: peekFront
gets the front of this queue. The operation does not modify the queue.

Input: None.
Output: The front of the queue.
return: false if Queue is empty
*/
template < typename T>
bool PriorityQueue<T>::peekFront(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;
}
///////////////////////////////////////////////////////////////////////////////////

/*
Function: DeleteNode.
Deletes a node from the Queue that have that wanted item.
Parameters:
- item : The item to be deleted from the Queue.
*/
template < typename T>
void PriorityQueue<T>::DeleteNode(const int& item)
{
	if (!frontPtr)
		return;

	PriorityNode<T>* p = frontPtr;
	if (p->getItem()->GetID() == item)  //if the first node have the wanted item
	{
		frontPtr = frontPtr->getNext();
		p->setNext(nullptr);
		delete p;
		return;
	}

	p = frontPtr->getNext();
	PriorityNode<T>* prev = frontPtr;
	while (p)
	{
		if (p->getItem()->GetID() == item)
		{
			prev->setNext(p->getNext());
			delete p;
			return;
		}
		prev = p;
		p = p->getNext();
	}
}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: toArray
returns an array of "T"
Output: count: the length of the returned array (zero if Queue is empty)
returns: The array of T. (nullptr if Queue is empty)
*/
template < typename T>
T* PriorityQueue<T>::toArray(int& count)
{
	count = 0;

	if (!frontPtr)
		return nullptr;

	//counting the no. of items in the Queue
	PriorityNode<T>* p = frontPtr;
	while (p)
	{
		count++;
		p = p->getNext();
	}

	T* Arr = new T[count];
	p = frontPtr;
	for (int i = 0; i < count; i++)
	{
		Arr[i] = p->getItem();
		p = p->getNext();
	}
	return Arr;
}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: SearchForOrder
Searchs for the order with the sent id and deletes it
Output: the deleted order
returns: a boolean if the order with the sent id was found and deleted or not
*/
template < typename T>
PriorityNode<T>* PriorityQueue<T>::SearchForOrder(T Ord)
{
	PriorityNode<T>* Temp = frontPtr;
	while (Temp)
	{
		if (Temp->getItem() == Ord)
			return Temp;
		Temp = Temp->getNext();
	}
	return Temp;
}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: ~PriorityQueue()
The destructor of the PriorityQueue class.
*/
template < typename T>
PriorityQueue<T>::~PriorityQueue<T>()
{
	T ptr;
	while (dequeue(ptr));
}
/////////////////////////////////////////////////////////////////////////////////////////