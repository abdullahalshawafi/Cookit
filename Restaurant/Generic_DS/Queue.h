#pragma once
/*This code is an updated version from "Data Abstraction & Problem Solving with C++,WALLS AND MIRRORS ,SIXTH EDITION"*/

/*
This is a program that implements the queue abstract data type using a linked list.
The queue is implemented as a chain of linked nodes that has two pointers,
a frontPtr pointer for the front of the queue and a backPtr pointer for the back of the queue.
*/

/*

				The Node: item of type T and a "next" pointer
					-------------
					| item| next | --->
					-------------
General Queue case:

				 frontPtr																backPtr
					\											   						/
					 \											  					   /
					------------- 	  ------------- 	  ------------- 	  -------------
					| item| next |--->| item| next |--->  | item| next |--->  | item| next |---> NULL
					------------- 	  ------------- 	  ------------- 	  -------------

Empty Case:

				 frontptr	 backptr
						\	 /
						 \	/
					---- NULL ------


Single Node Case:
				 frontPtr	 backPtr
					\		/
					 \	   /
					--------
					|	|nxt -->NULL
					--------

*/

#include <iostream>
#include "Node.h"
#include "../Events/Event.h"

template <typename T>
class Queue
{
private:
	Node<T>* backPtr;
	Node<T>* frontPtr;
public:
	Queue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry);
	bool dequeue(T& frntEntry);
	bool peekFront(T& frntEntry)  const;
	T* toArray(int& count);
	bool SearchForOrder(int id, T& Entry);
	int GetQueueSize();
	~Queue();
};
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: Queue()
The constructor of the Queue class.

*/
template <typename T>
Queue<T>::Queue()
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
template <typename T>
bool Queue<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}
/////////////////////////////////////////////////////////////////////////////////////////

/*Function:enqueue
Adds newEntry at the back of this queue.

Input: newEntry .
Output: True if the operation is successful; otherwise false.
*/
template <typename T>
bool Queue<T>::enqueue(const T& newEntry)
{
	Node<T>* newNodePtr = new Node<T>(newEntry);
	// Insert the new node
	if (isEmpty())
		frontPtr = newNodePtr; // The queue is empty
	else
		backPtr->setNext(newNodePtr); // The queue was not empty
	backPtr = newNodePtr; // New node is at back
	return true;
} // end enqueue
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Function: dequeue
Removes the front of this queue. That is, removes the item that was added
earliest.

Input: None.
Output: True if the operation is successful; otherwise false.
*/
template <typename T>
bool Queue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = frontPtr;
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
return: flase if Queue is empty
*/
template <typename T>
bool Queue<T>::peekFront(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;
}
///////////////////////////////////////////////////////////////////////////////////

template <typename T>
Queue<T>::~Queue()
{
}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: toArray
returns an array of "T"
Output: size: the length of the returned array (zero if Queue is empty)
returns: The array of T. (nullptr if Queue is empty)
*/
template <typename T>
T* Queue<T>::toArray(int& size)
{
	int count = GetQueueSize();
	T* Arr = new T[count];
	Node<T>* p = frontPtr;
	for (int i = 0; i < count; i++)
	{
		Arr[i] = p->getItem();
		p = p->getNext();
	}
	size = count;
	return Arr;
}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: SearchForOrder
Searchs for the order with the sent id and deletes it
Output: the deleted order
returns: a boolean if the order with the sent id was found and deleted or not
*/
template <typename T>
bool Queue<T>::SearchForOrder(int id, T& Entry)
{
	Node<T>* Temp = frontPtr;
	Node<T>* Nodetodelete = NULL;
	if (isEmpty())  //the queue is Empty
		return false;

	if (Temp->getItem()->GetID() == id)  //if the first node have the wanted order
	{
		Nodetodelete = Temp;
		Entry = Temp->getItem();
		frontPtr = Temp->getNext();
		delete Nodetodelete;
		return true;
	}

	while (Temp && Temp->getNext())
	{
		if (Temp->getNext()->getItem()->GetID() == id) //if the next node have the wanted order
		{
			Nodetodelete = Temp->getNext();
			Entry = Temp->getNext()->getItem();
			Temp->setNext(Temp->getNext()->getNext());
			delete Nodetodelete;
			if (!Temp->getNext())
				backPtr = Temp;
			return true;
		}
		Temp = Temp->getNext();
	}

	return false;
}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: GetQueueSize
Calculates the size og the queue
Output: queue size
returns: integer representing the size of the queue
*/
template <typename T>
int Queue<T>::GetQueueSize()
{
	int count = 0;

	//counting the no. of items in the Queue
	Node<T>* p = frontPtr;
	while (p)
	{
		count++;
		p = p->getNext();
	}
	return count;
}
/////////////////////////////////////////////////////////////////////////////////////////