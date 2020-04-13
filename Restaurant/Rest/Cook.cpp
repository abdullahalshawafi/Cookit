#include "Cook.h"

int Cook::OrdToBreak = 0;

Cook::Cook()
{
	CurrOrd = 0;
	InBreak = false;
	AssignedOrder = nullptr;
}

void Cook::SetID(int id)
{
	ID = id;
}

int Cook::GetID() const
{
	return ID;
}

void Cook::SetType(ORD_TYPE Cook_Type)
{
	type = Cook_Type;
}

ORD_TYPE Cook::GetType() const
{
	return type;
}

void Cook::SetSpeed(int Speed)
{
	speed = Speed;
}

int Cook::GetSpeed() const
{
	return speed;
}

void Cook::SetBreakDuration(int breakDuration)
{
	BreakDuration = breakDuration;
}

int Cook::GetBreakDuration() const
{
	return BreakDuration;
}

void Cook::SetCurrOrd(int OrderID)
{
	CurrOrd = OrderID;
}

int Cook::GetCurrOrd() const
{
	return CurrOrd;
}

void Cook::SetInBreak(bool inBreak)
{
	InBreak = inBreak;
}

bool Cook::GetInBreak() const
{
	return InBreak;
}

void Cook::SetAssignedOrder(Order* order)
{
	AssignedOrder = order;
}

Order* Cook::GetAssignedOrder() const
{
	return AssignedOrder;
}

void Cook::SetOrdToBreak(int OB)
{
	OrdToBreak = OB;
}

int Cook::GetOrdToBreak()
{
	return OrdToBreak;
}

Cook::~Cook()
{
}