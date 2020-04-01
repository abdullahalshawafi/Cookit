#include "CancellationEvent.h"
#include <iostream>
using namespace std;

CancellationEvent::CancellationEvent(int eTime, int oID) :Event(eTime, oID)
{
	id = oID;
}
//Add more constructors if needed

void CancellationEvent::Execute(Restaurant* pRest)//override execute function
{
	pRest->DeleteNormalQueue(id);
}