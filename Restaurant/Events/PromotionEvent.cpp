#include "PromotionEvent.h"
PromotionEvent::PromotionEvent(int eTime, int O_id, double extraM) :Event(eTime, O_id)
{
	OrderID = O_id;
	ExtraMoney = extraM;
}
void PromotionEvent::Execute(Restaurant* pRest)
{
	Order* pOrder = pRest->PromotOrder(OrderID, ExtraMoney);
	if (pOrder)
		pRest->AddtoVIPQueue(pOrder);
}