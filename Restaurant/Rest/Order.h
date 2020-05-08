#pragma once

#include "..\Defs.h"
#include "Cook.h"

class Cook;

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	int OrderSize;		//No. of order's dishes
	double totalMoney;	//Total order money
	int ArrTime, ServTime, FinishTime;	//arrival, service start, and finish times
	Cook* AssignedCook; //Pointer to the cook assigned for the order

public:
	Order();
	Order(int arrTime, int id, ORD_TYPE O_Type, int O_Size, double O_Money);

	void SetID(int id);
	int GetID() const;

	void SetType(ORD_TYPE Order_Type);
	ORD_TYPE GetType() const;

	void SetStatus(ORD_STATUS Status);
	ORD_STATUS GetStatus() const;

	void SetOrderSize(int size);
	int GetOrderSize() const;

	void SetOrderMoney(double Money);
	double GetOrderMoney() const;

	void SetArrivalTime(int arrTime);
	int GetArrivalTime() const;

	void SetInServiceTime(int sServiceTime);
	int GetInServiceTime() const;

	void SetFinishTime(int finishTime);
	int GetFinishTime() const;

	void SetAssignedCook(Cook* cook);
	Cook* GetAssignedCook() const;

	virtual ~Order();
};
