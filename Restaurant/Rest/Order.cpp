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

void Order::SetType(ORD_TYPE O)
{
	type = O;
}

void Order::SetOrderMoney(double M)
{
	totalMoney = M;
}