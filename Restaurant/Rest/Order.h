#pragma once
#include "..\Defs.h"
#include "Cook.h"

class Cook;

class Order
{
	int ID;								//Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;						//order type: Normal, vegan, VIP
	ORD_STATUS status;					//Waiting, In-Service, and Done
	int ArrTime, ServTime, FinishTime;	//Arrival, Service start, and Finish Timesteps
	int OrderSize;						//No. of dishes in order
	int PromotionTS;					//The Timestep in which the order has been promoted to VIP
	double totalMoney;					//Total order money
	bool Urgent;						//Is the order urgent or not
	static int VIP_Wait;				//No. of Timesteps an VIP order should wait before being urgent
	static int AutoPro;					//No. of Timsteps a Normal order should wait before being autopromoted to VIP

public:
	Order();
	Order(int arrTime, int id, ORD_TYPE O_Type, int O_Size, double O_Money);

	void SetID(int id);
	int GetID() const;

	void SetType(ORD_TYPE Order_Type);
	ORD_TYPE GetType() const;

	void SetStatus(ORD_STATUS Status);
	ORD_STATUS GetStatus() const;

	void SetArrivalTime(int arrTime);
	int GetArrivalTime() const;

	void SetInServiceTime(int sServiceTime);
	int GetInServiceTime() const;

	void SetFinishTime(int finishTime);
	int GetFinishTime() const;

	void SetOrderSize(int size);
	int GetOrderSize() const;

	void SetPromotionTS(int TS);
	int GetPromotionTS() const;

	void SetOrderMoney(double Money);
	double GetOrderMoney() const;

	void SetUrgent(bool ans);
	bool GetUrgent() const;

	static void SetVIP_WAITANDNRM_WAIT(int, int);	//First parameter is VIP_wait and second one is for Normal wait to be promoted
	static int GetVIP_WAITANDNRM_WAIT(int&);		//returns VIP_wait and setes the parameter by Normal wait to be promoted

	virtual ~Order();
};
