#pragma once
#include "Node.h"
#include <iostream>
#include <time.h>
using namespace std;

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
	}
	////////////////////////////////////////////////////////////////////////

	/*
	* Function: InsertEnd
	* Creates a new node and adds it to the endf a linked list.
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
	* Function: RemoveFirst
	* removes and returns the first node in the linked list.
	*/
	Node<T>* RemoveFirst()
	{
		Node<T>* p = Head;
		if (Head)
		{
			Head = Head->getNext();
			p->setNext(nullptr);
		}
		return p;
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
	* Function: RemoveFinishedFirst.
	* Finds and returns the node having the minimum finish time and deletes it.
	*/
	T RemoveFinishedFirst() {
		if (!Head)
			return nullptr;

		T minFinish;
		Node<T>* p = Head;
		if (!p->getNext()) // only one node
		{
			minFinish = p->getItem();
			delete p;
			return minFinish;
		}

		minFinish = Head->getItem();
		p = p->getNext();
		while (p)   //getting order of minimum finish time 
		{
			if (p->getItem()->getFinishTime() < minFinish->getFinishTime())
				minFinish = p->getItem();
			p = p->getNext();
		}

		DeleteNode(minFinish); //removing fisrt finished order from its list
		return minFinish;
	}
	////////////////////////////////////////////////////////////////////////

	/*
	* Function: Remove.
	* Deletes and returns the last item in the linked list.
	*/
	T Remove()    //removes any order (will be used in phase 1 only)
	{
		if (!Head)
			return nullptr;

		T item;
		Node<T>* p = Head;
		Node<T>* prev = Head;
		if (!p->getNext()) // only one node
		{
			item = p->getItem();
			delete Head;
			Head = nullptr;
			return item;
		}

		while (p && p->getNext())
		{
			prev = p;
			p = p->getNext();
		}

		item = p->getItem();
		delete p;
		p = nullptr;
		prev->setNext(nullptr);
		return item;
	}
	////////////////////////////////////////////////////////////////////////

	void InsertSorted(Node<T>* Cook) //For Cooks
	{
		if (!Head)
		{
			Head = Cook;
			return;
		}

		if (Head->getItem()->GetTimeToBeFree() >= Cook->getItem()->GetTimeToBeFree())
		{
			Cook->setNext(Head);
			Head = Cook;
			return;
		}

		Node<T>* p = Head;
		Node<T>* After = Head->getNext();

		while (After && After->getItem()->GetTimeToBeFree() < Cook->getItem()->GetTimeToBeFree())
		{
			p = p->getNext();
			After = After->getNext();
		}

		if (After)
		{
			Cook->setNext(p->getNext());
			p->setNext(Cook);
			return;
		}

		p->setNext(Cook);
		return;
	}
	////////////////////////////////////////////////////////////////////////

	Node<T>* GetTheFirstNRMCook() //To Get First NRMCook in The list Of Cooks not in break
	{
		Node<T>* p = Head;
		if (!Head) return nullptr;

		if (Head->getItem()->GetType() == TYPE_NRM)
		{
			if (Head->getItem()->GetTimeToBeFree() == 0)
			{
				Head = Head->getNext();
				p->setNext(nullptr);
				return p;
			}
			return nullptr;
		}

		Node<T>* After = Head->getNext();

		while (After && After->getItem()->GetType() != TYPE_NRM)
		{
			p = p->getNext();
			After = After->getNext();
		}

		if (!After) return nullptr;

		if (After->getItem()->GetTimeToBeFree() == 0)
		{
			p->setNext(After->getNext());
			After->setNext(nullptr);
			return After;
		}

		return nullptr;
	}
	////////////////////////////////////////////////////////////////////////

	Node<T>* GetTheFirstVGNCook()//To Get First VGNCook in The list Of Cooks not in break
	{
		Node<T>* p = Head;
		if (!Head) return nullptr;

		if (Head->getItem()->GetType() == TYPE_VGAN)
		{
			if (Head->getItem()->GetTimeToBeFree() == 0)
			{
				Head = Head->getNext();
				p->setNext(nullptr);
				return p;
			}
			return nullptr;
		}

		Node<T>* After = Head->getNext();

		while (After && After->getItem()->GetType() != TYPE_VGAN)
		{
			p = p->getNext();
			After = After->getNext();
		}

		if (!After) return nullptr;

		if (After->getItem()->GetTimeToBeFree() == 0)
		{
			p->setNext(After->getNext());
			After->setNext(nullptr);
			return After;
		}

		return nullptr;
	}
	////////////////////////////////////////////////////////////////////////

	Node<T>* GetTheFirstVIPCook()//To Get First VIPCook in The list Of Cooks not in break
	{
		Node<T>* p = Head;
		if (!Head) return nullptr;

		if (Head->getItem()->GetType() == TYPE_VIP)
		{
			if (Head->getItem()->GetTimeToBeFree() == 0)
			{
				Head = Head->getNext();
				p->setNext(nullptr);
				return p;
			}
			return nullptr;
		}

		Node<T>* After = Head->getNext();

		while (After && After->getItem()->GetType() != TYPE_VIP)
		{
			p = p->getNext();
			After = After->getNext();
		}

		if (!After) return nullptr;

		if (After->getItem()->GetTimeToBeFree() == 0)
		{
			p->setNext(After->getNext());
			After->setNext(nullptr);
			return After;
		}

		return nullptr;
	}
	////////////////////////////////////////////////////////////////////////

	Node<T>* GetTheFirstUnAvailableCook()//To Get the first unavailable Cook
	{
		Node<T>* p = Head;
		if (!Head) return Head;

		if (Head->getItem()->GetTimeToBeFree() != 0)
		{
			Head = Head->getNext();
			p->setNext(nullptr);
			return p;
		}

		Node<T>* After = Head->getNext();

		while (After && After->getItem()->GetTimeToBeFree() == 0)
		{
			p = p->getNext();
			After = After->getNext();
		}

		if (!After) return After;

		p->setNext(After->getNext());
		After->setNext(nullptr);

		return After;
	}
	////////////////////////////////////////////////////////////////////////

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
