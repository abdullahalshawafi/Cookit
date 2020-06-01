#include "Cook.h"

int Cook::OrdToBreak = 0;
float Cook::InjuryPro = 0;
int Cook::RestPeriod = 0;

Cook::Cook()
{
	FinishedOrders = 0;
	InBreak = false;
	AssignedOrder = nullptr;
	BackFromRest = 0;
	Injured = false;
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

void Cook::SetFinishedOrders(int OrderID)
{
	FinishedOrders = OrderID;
}

int Cook::GetFinishedOrders() const
{
	return FinishedOrders;
}

void Cook::SetBreakTS(int TS)
{
	BreakTimestep = TS;
}

int Cook::GetBreakTS()
{
	return BreakTimestep;
}

void Cook::SetOriginalSpeed(int s)
{
	OriginalSpeed = s;
}

void Cook::ResetSpeed()
{
	speed = OriginalSpeed;
}

void Cook::SetBackFromRest(int F)
{
	BackFromRest = F;
}

int Cook::GetBackFromRest()
{
	return BackFromRest;
}

void Cook::SetInBreak(bool inBreak)
{
	InBreak = inBreak;
}

bool Cook::GetInBreak() const
{
	return InBreak;
}

void Cook::SetInjured(bool i)
{
	Injured = i;
}

bool Cook::GetInjured()
{
	return Injured;
}

void Cook::SetAssignedOrder(Order* order)
{
	AssignedOrder = order;
}

Order* Cook::GetAssignedOrder() const
{
	return AssignedOrder;
}

void Cook::SetOrdToBreakANDRest(int OB, int restd)
{
	OrdToBreak = OB;
	RestPeriod = restd;
}

int Cook::GetOrdToBreakANDRest(int& restd)
{
	restd = RestPeriod;
	return OrdToBreak;
}

void Cook::SetInjProp(float i)
{
	InjuryPro = i;
}

float Cook::GetInjProp()
{
	return InjuryPro;
}

Cook::~Cook()
{
}