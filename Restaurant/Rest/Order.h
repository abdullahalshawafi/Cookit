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
	bool Urgent;
	static int VIP_Wait;	//number of allowed time to wait
	static int AutoPro;  //number of allowed time to remain normal

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


	void SetUrgent(bool ans);
    bool GetUrgent() const;

	void SetArrivalTime(int arrTime);
	int GetArrivalTime() const;

	void SetInServiceTime(int sServiceTime);
	int GetInServiceTime() const;

	void SetFinishTime(int finishTime);
	int GetFinishTime() const;
	static void SetVIP_WAITANDNRM_WAIT( int ,int);//First parameter is VIP_wait & second one is for Normal wait to be promoted
	static int GetVIP_WAITANDNRM_WAIT(int &); //returns VIP_wait and setes the parameter by Normal wait to be promoted



	virtual ~Order();
};
