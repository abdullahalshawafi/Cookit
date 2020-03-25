#pragma once

#include "Event.h"
#include "..\Rest\Restaurant.h"


//class for the arrival event
class ArrivalEvent : public Event
{
	//info about the order related to arrival event
	int OrdDistance;	//order distance
	ORD_TYPE OrdType;	//order type: Normal, vegan, VIP	                
	double OrdMoney;	//Total order money
	int OrdSize;
public:
	ArrivalEvent(int eTime, int oID, ORD_TYPE oType, int OS, double tMoney);
	//Add more constructors if needed

	virtual void Execute(Restaurant* pRest);	//override execute function

};
