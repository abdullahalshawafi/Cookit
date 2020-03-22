#pragma once
#include "Event.h"
#include"../Rest/Restaurant.h"

class PromotionEvent : public Event
{
public:
	PromotionEvent(int eTime, int O_id, double extraM);
	virtual void Execute(Restaurant* pRest);
};

