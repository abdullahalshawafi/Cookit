#include"Order.h"

Order::Order()
{
	AssignedCook = nullptr;
}

Order::Order(int arrTime, int id, ORD_TYPE O_Type, int O_Size, double O_Money)
{
	ArrTime = arrTime;
	ID = (id > 0 && id < 1000) ? id : 0;	//1<ID<999
	type = O_Type;
	status = WAIT;
	OrderSize = O_Size;
	totalMoney = O_Money;
	AssignedCook = nullptr;
}

void Order::SetID(int id)
{
	ID = (id > 0 && id < 1000) ? id : 0;	//1<ID<999
}

int Order::GetID() const
{
	return ID;
}

void Order::SetType(ORD_TYPE Order_Type)
{
	type = Order_Type;
}

ORD_TYPE Order::GetType() const
{
	return type;
}

void Order::SetStatus(ORD_STATUS Status)
{
	status = Status;
}

ORD_STATUS Order::GetStatus() const
{
	return status;
}

void Order::SetOrderSize(int size)
{
	OrderSize = size;
}

int Order::GetOrderSize() const
{
	return OrderSize;
}

void Order::SetOrderMoney(double Money)
{
	totalMoney = Money;
}

double Order::GetOrderMoney() const
{
	return totalMoney;
}

void Order::SetArrivalTime(int arrTime)
{
	ArrTime = arrTime;
}

int Order::GetArrivalTime() const
{
	return ArrTime;
}

void Order::SetInServiceTime(int ServiceTime)
{
	ServTime = ServiceTime;
}

int Order::GetInServiceTime() const
{
	return ServTime;
}

void Order::SetFinishTime(int finishTime)
{
	FinishTime = finishTime;
}

int Order::GetFinishTime() const
{
	return FinishTime;
}

void Order::SetAssignedCook(Cook* cook)
{
	AssignedCook = cook;
}

Cook* Order::GetAssignedCook() const
{
	return AssignedCook;
}

Order::~Order()
{

}