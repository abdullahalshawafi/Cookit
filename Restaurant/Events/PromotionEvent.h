#pragma once
#include "Event.h"
#include"../Rest/Restaurant.h"

class PromotionEvent : public Event
{
	int OrderID;
	double ExtraMoney;
public:
	PromotionEvent(int eTime, int O_id, double extraM);
	virtual void Execute(Restaurant* pRest);
};

