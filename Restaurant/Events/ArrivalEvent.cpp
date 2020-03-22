#include "ArrivalEvent.h"


ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType, int OS, double tMoney) :Event(eTime, oID)
{
	OrdType = oType;
	OrdMoney = tMoney;
	OrdSize = OS;
}

void ArrivalEvent::Execute(Restaurant* pRest)
{
	//This function should create an order and fills its data 
	// Then adds it to normal, vegan, or VIP order lists that you will create in phase1
	Order* pOrd = new Order(OrderID, OrdType, OrdMoney, OrdSize);
	if (OrdType == TYPE_NRM)
		pRest->AddtoNormalQueue(pOrd);
	if (OrdType == TYPE_VGAN)
		pRest->AddtoVeganQueue(pOrd);
	if (OrdType == TYPE_VIP)
		pRest->AddtoVIPQueue(pOrd);


	///For the sake of demo, this function will just create an order and add it to DemoQueue
	///Remove the next code lines in phases 1&2
}