#include "PromotionEvent.h"
PromotionEvent::PromotionEvent(int eTime, int O_id, double extraM) : Event(eTime, O_id)
{
	ExtraMoney = extraM;
}
void PromotionEvent::Execute(Restaurant* pRest)
{
	Order* pOrd = pRest->PromotOrder(getOrderID(), ExtraMoney);
	if (pOrd)
		pRest->AddtoVIPQueue(pOrd);
}