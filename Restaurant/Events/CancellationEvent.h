#pragma once
#include "../Rest/Restaurant.h"
#include "../GUI/GUI.h"
#include "Event.h"

class Restaurant;
class Event;

class CancellationEvent : public Event
{
public:
	CancellationEvent(int eTime, int oID);
	virtual void Execute(Restaurant* pRest);	//override execute function
};


