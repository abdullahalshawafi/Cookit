#include "ArrivalEvent.h"

ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType, int oSize, double totalMoney) : Event(eTime, oID)
{
	OrdType = oType;
	OrdMoney = totalMoney;
	OrdSize = oSize;
}

//This function creates an order and fills its data 
//Then adds it to normal, vegan, or VIP order lists
void ArrivalEvent::Execute(Restaurant* pRest)
{
	Order* pOrd = new Order(getEventTime(), getOrderID(), OrdType, OrdSize, OrdMoney);
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