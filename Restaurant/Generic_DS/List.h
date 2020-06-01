#pragma once
#include "Node.h"

template <typename T>
class List
{
private:
	Node<T>* Head;	//Pointer to the head of the list
public:
	List()
	{
		Head = nullptr;
	}
	////////////////////////////////////////////////////////////////////////

	//When List is being desturcted ==> delete all items in the list
	~List()
	{
		DeleteAll();
	}
	////////////////////////////////////////////////////////////////////////

	/*
	*Function: IsEmpty
	*returns: boolean representing the linked list is empty or not
	*/
	bool IsEmpty()
	{
		return (!Head);
	}
	////////////////////////////////////////////////////////////////////////

	/*
	*Function: GetCount
	*Calculates the size og the linked list
	*Output: linked list size
	*returns: integer representing the size of the linked list
	*/
	int GetCount()
	{
		int count = 0;

		//counting the no. of items in the linked list
		Node<T>* p = Head;
		while (p)
		{
			count++;
			p = p->getNext();
		}
		return count;
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
	}
	////////////////////////////////////////////////////////////////////////

	/*
	* Function: DeleteNode.
	* Deletes a node from the lined list that have that wanted item.
	*
	* Parameters:
	*	- item : The item to be deleted from the linked list.
	*/
	void DeleteNode(const T& item)
	{
		if (!Head)
			return;

		Node<T>* p = Head;
		if (p->getItem() == item)  //if the first node have the wanted item
		{
			Head = Head->getNext();
			p->setNext(nullptr);
			delete p;
			return;
		}

		p = Head->getNext();
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
	////////////////////////////////////////////////////////////////////////

	/*
	* Function: RemoveFirst
	* removes and returns the first node in the linked list.
	*/
	T RemoveFirst()
	{
		if (IsEmpty()) return nullptr;
		Node<T>* p = Head;
		Head = Head->getNext();
		T item = p->getItem();
		p->setNext(nullptr);
		delete p;
		return item;
	}
	////////////////////////////////////////////////////////////////////////

	/*
	* Function: InsertEnd
	* Creates a new node and adds it to the end of a linked list.
	*
	* Parameters:
	*	- data : The value to be stored in the new node.
	*/
	void InsertEnd(const T& data)
	{
		Node<T>* ptr = Head;
		if (!Head)  //if the list is empty
		{
			Node<T>* R = new Node<T>(data);
			Head = R;
			Head->setNext(NULL);
			return;
		}

		while (ptr && ptr->getNext())
			ptr = ptr->getNext();
		Node<T>* R = new Node<T>(data);
		ptr->setNext(R);
		R->setNext(NULL);
	}
	////////////////////////////////////////////////////////////////////////

	/*
	* Function: InsertSorted
	* Creates a new node and adds it in the sorted place in the linked list.
	*
	* Parameters:
	*	- data : The value to be stored in the new node.
	*/
	void InsertSorted(T data) //It is used for sorting Cooks
	{
		Node<T>* p = Head;
		Node<T>* pCook = new Node<T>(data);
		if (!Head)
		{
			Head = pCook;
			return;
		}
		Node<T>* After = Head->getNext();

		if (Head->getItem()->GetFinishedOrders() > pCook->getItem()->GetFinishedOrders())
		{
			pCook->setNext(Head);
			Head = pCook;
			return;
		}

		while (After && After->getItem()->GetFinishedOrders() <= pCook->getItem()->GetFinishedOrders())
		{
			p = p->getNext();
			After = After->getNext();
		}

		if (After)
		{
			pCook->setNext(p->getNext());
			p->setNext(pCook);
			return;
		}

		p->setNext(pCook);
		return;
	}
	////////////////////////////////////////////////////////////////////////

	/*
	* Function: toArray
	* Returns an array of "T"
	*
	* Parameters:
	*	- size : the length of the returned array (zero if Queue is empty).
	*/
	T* List<T>::toArray(int& size) const
	{
		int count = 0;

		//counting the no. of items in the linked list
		Node<T>* p = Head;
		while (p)
		{
			count++;
			p = p->getNext();
		}

		size = count;
		if (!count)
			return nullptr;

		T* retArr = new T[count];
		p = Head;
		for (int i = 0; i < count; i++)
		{
			retArr[i] = p->getItem();
			p = p->getNext();
		} // end for loop
		return retArr;
	}
	////////////////////////////////////////////////////////////////////////

	/*
	* Function: SearchForOrder
	* Searchs for the order with the sent id and deletes it
	* Output: the deleted order
	* returns: a boolean if the order with the sent id was found and deleted or not
	*/
	bool SearchForOrder(int id, T& Entry)
	{
		Node<T>* Temp = Head;
		Node<T>* Nodetodelete = nullptr;
		if (!Head)  //the queue is Empty
			return false;

		if (Temp->getItem()->GetID() == id)  //if the first node have the wanted order
		{
			Nodetodelete = Temp;
			Entry = Temp->getItem();
			Head = Temp->getNext();
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
				return true;
			}
			Temp = Temp->getNext();
		}

		return false;
	}
	////////////////////////////////////////////////////////////////////////
};
