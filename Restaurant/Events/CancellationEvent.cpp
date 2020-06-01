#include "CancellationEvent.h"
#include <iostream>
using namespace std;

CancellationEvent::CancellationEvent(int eTime, int oID) : Event(eTime, oID)
{
}

//This function excutes the cancelation event 
//and deletes a Normal order from normal orders list
void CancellationEvent::Execute(Restaurant* pRest)
{
	bool cancelled = pRest->DeletefromNormalQueue(getOrderID());
}