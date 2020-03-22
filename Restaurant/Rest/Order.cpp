#include"Order.h"

Order::Order(int id, ORD_TYPE r_Type, double OM, int OS)
{
	ID = (id > 0 && id < 1000) ? id : 0;	//1<ID<999
	type = r_Type;
	status = WAIT;
	totalMoney = OM;
	OrderSize = OS;
}
Order::Order()
{
}
Order::~Order()
{
}

int Order::GetID()
{
	return ID;
}


ORD_TYPE Order::GetType() const
{
	return type;
}


void Order::SetDistance(int d)
{
	Distance = d > 0 ? d : 0;
}

int Order::GetDistance() const
{
	return Distance;
}


void Order::setStatus(ORD_STATUS s)
{
	status = s;
}

ORD_STATUS Order::getStatus() const
{
	return status;
}

void Order::setSize(int s)
{
	OrderSize = s;
}

int Order::getSize()
{
	return OrderSize;
}

bool Order::operator==(Order Ord)
{
	if (ID == Ord.GetID())
		return true;
	else
		return false;


}

int Order::GetArrivalTime()
{
	return ArrTime;
}

int Order::GetOrderSize()
{
	return OrderSize;
}

double Order::GetOrderMoney()
{
	return totalMoney;
}

int Order::getFinishTime()
{
	return FinishTime;
}

int Order::getInServiceTime()
{
	return ServTime;
}

void Order::SetArrivalTime(int at)
{
	ArrTime = at;
}

void Order::SetInServiceTime(int st)
{
	ServTime = st;
}

void Order::SetFinishTime(int ft)
{
	FinishTime = ft;
}