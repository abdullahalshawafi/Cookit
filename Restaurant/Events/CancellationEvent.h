#pragma once
#include"../Rest/Restaurant.h"
#include "../GUI/GUI.h"
#include"Event.h"
class Restaurant;
class Event;
class CancellationEvent : public Event
{
	int eTime;
	int oID;


public:
	CancellationEvent(int eTime, int oID);
	//Add more constructors if needed

	virtual void Execute(Restaurant* pRest);	//override execute function

};


