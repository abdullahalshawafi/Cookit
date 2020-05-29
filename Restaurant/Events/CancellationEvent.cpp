#include "CancellationEvent.h"
#include <iostream>
using namespace std;

CancellationEvent::CancellationEvent(int eTime, int oID) :Event(eTime, oID)
{
	OrderID = oID;
}
//Add more constructors if needed

void CancellationEvent::Execute(Restaurant* pRest)//override execute function
{
	bool cancelled = pRest->DeleteNormalQueue(OrderID);
	if (cancelled)
		cout << "\nOrder " << OrderID << " has been cancelled" << endl;
}