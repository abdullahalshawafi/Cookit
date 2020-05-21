#pragma once

#include "Event.h"
#include "..\Rest\Restaurant.h"


//class for the arrival event
class ArrivalEvent : public Event
{
	//info about the order related to arrival event
	ORD_TYPE OrdType;	//order type: Normal, vegan, VIP	                
	double OrdMoney;	//Total order money
	int OrdSize;        //No. of order's dishes

public:
	ArrivalEvent(int eTime, int oID, ORD_TYPE oType, int oSize, double totalMoney);
	virtual void Execute(Restaurant* pRest);	//override execute function

};
