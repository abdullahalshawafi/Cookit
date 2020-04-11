#pragma once

#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	int OrderSize;
	double totalMoney;	//Total order money
	int ArrTime, ServTime, FinishTime;	//arrival, service start, and finish times


	//
	// TODO: Add More Data Members As Needed
	//

public:
	Order();
	Order(int id, ORD_TYPE r_Type, double OM, int OS);
	virtual ~Order();

	int GetID();
	ORD_TYPE GetType() const;
	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;
	void setSize(int s);
	int getSize();
	int getFinishTime();
	int getInServiceTime();
	void SetArrivalTime(int at);
	void SetInServiceTime(int st);
	void SetFinishTime(int ft);
	int GetArrivalTime();
	int GetOrderSize();
	double GetOrderMoney();
	//
	// TODO: Add More Member Functions As Needed
	//

};
