#pragma once
#include"Node.h"
#include <iostream>
#include <time.h>
using namespace std;

template <typename T>
class List
{
private:
	Node<T>* Head;	//Pointer to the head of the list
	int count;	//Number of nodes in the list
public:
	List()
	{
		count = 0;
		Head = nullptr;
	}
	// creates linked list from the passed array

	//List is being desturcted ==> delete all items in the list
	~List()
	{
		DeleteAll();
	}
	////////////////////////////////////////////////////////////////////////

	void DeleteNode(T& item)
	{
		if (!Head)
			return;
		if (Head->getItem == item)
		{
			delete Head;
			Head = nullptr;
			count--;
			return;
		}
		Node<T>* p = Head->getNext();
		Node<T>* prev = Head;
		while (p)
		{
			if (p->getItem() == item)
			{
				prev->setNext(p->getNext());
				delete p;
				return;
			}
			prev = p;
			p = p->getNext();
		}
	}

	/*
	* Function: InsertBeg.
	* Creates a new node and adds it to the beginning of a linked list.
	*
	* Parameters:
	*	- data : The value to be stored in the new node.
	*/
	void InsertBeg(const T& data)
	{
		Node<T>* R = new Node<T>(data);
		R->setNext(Head);
		Head = R;
		count++;
	}
	////////////////////////////////////////////////////////////////////////

	/*
	* Function: DeleteAll.
	* Deletes all nodes of the list.
	*/
	void DeleteAll()
	{
		Node<T>* P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
		count = 0;
	}

	int getcount()
	{
		return count;
	}
	////////////////////////////////////////////////////////////////////////

	//InsertEnd 
	//inserts a new node at end if the list
	void InsertEnd(const T& data)
	{
		Node<T>* ptr = Head;
		if (!Head)
		{
			Node<T>* R = new Node<T>(data);
			Head = R;
			Head->setNext(NULL);
			count++;
		}
		else
		{
			while (ptr && ptr->getNext())
				ptr = ptr->getNext();
			Node<T>* R = new Node<T>(data);
			ptr->setNext(R);
			R->setNext(NULL);
			count++;
		}
	}
	////////////////////////////////////////////////////////////////////////

	//RemoveFirst
	//removes and returns the first node from the list
	Node<T>* RemoveFirst()
	{
		Node<T>* p = Head;
		if (Head)
		{
			Head = Head->getNext();
			count--;
			p->setNext(nullptr);
		}
		return p;
	}
	////////////////////////////////////////////////////////////////////////

	T* List<T>::toArray(int& size) const
	{
		T* retArr = new T[count];
		Node<T>* curPtr = Head;
		int i = 0;
		while (curPtr)
		{
			retArr[i++] = curPtr->getItem();
			curPtr = curPtr->getNext();
		} // end while
		size = count;
		return retArr;
	}

	T RemoveFinishedFirst() {
		if (!Head)
			return nullptr;

		Node<T>* p = Head;
		if (!p->getNext()) // only one node
		{
			Head = nullptr;
			count = 0;
			return p->getItem();
		}

		T minFinish = Head->getItem();
		p = p->getNext();
		while (p)   //getting order of minimum finish time 
		{
			if (p->getItem()->getFinishTime() < minFinish->getFinishTime())
				minFinish = p->getItem();
			p = p->getNext();
		}
		return minFinish;
	}

	T Remove()    //removes any order (will be used in phase 1 only)
	{
		if (!Head)
			return nullptr;

		Node<T>* p = Head;
		Node<T>* prev = Head;
		if (!p->getNext()) // only one node
		{
			Head = nullptr;
			count = 0;
			return p->getItem();
		}
		while (p && p->getNext())
		{
			prev = p;
			p = p->getNext();
		}
		prev->setNext(NULL);
		count--;
		return p->getItem();
	}
};

Order* List<Order*>::RemoveFinishedFirst()
{
	if (!Head)
		return nullptr;

	Node<Order*>* p = Head;
	if (!p->getNext()) // only one node
	{
		Head = nullptr;
		count = 0;
		p->getItem()->setStatus(DONE);
		return p->getItem();
	}

	Order* minFinish = Head->getItem();
	p = p->getNext();
	while (p)   //getting order of minimum finish time 
	{
		if (p->getItem()->getFinishTime() < minFinish->getFinishTime())
			minFinish = p->getItem();
		p = p->getNext();
	}

}

