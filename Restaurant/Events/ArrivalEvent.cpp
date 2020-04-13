#include "ArrivalEvent.h"

ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType, int oSize, double totalMoney) :Event(eTime, oID)
{
	OrdType = oType;
	OrdMoney = totalMoney;
	OrdSize = oSize;
}

void ArrivalEvent::Execute(Restaurant* pRest)
{
	//This function should create an order and fills its data 
	// Then adds it to normal, vegan, or VIP order lists

	Order* pOrd = new Order(EventTime, OrderID, OrdType, OrdSize, OrdMoney);
	switch (OrdType)
	{
	case TYPE_NRM:
		pRest->AddtoNormalQueue(pOrd);
		break;
	case TYPE_VGAN:
		pRest->AddtoVeganQueue(pOrd);
		break;
	case TYPE_VIP:
		pRest->AddtoVIPQueue(pOrd);
		break;
	default:
		break;
	}
}