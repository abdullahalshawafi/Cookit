#include "Cook.h"

int Cook::OrdToBreak = 0;

Cook::Cook()
{
	CurrOrd = 0;
	InBreak = false;
}


Cook::~Cook()
{
}


int Cook::GetID() const
{
	return ID;
}


ORD_TYPE Cook::GetType() const
{
	return type;
}

int Cook::GetSpeed() const
{
	return speed;
}


void Cook::setID(int id)
{
	ID = id;
}

void Cook::setType(ORD_TYPE t)
{
	type = t;
}

void Cook::setSpeed(int s)
{
	speed = s;
}

void Cook::setBO(int bo)
{
	OrdToBreak = bo;
}
void Cook::setBD(int bd)
{
	BreakDuration = bd;
}
int Cook::getBO()
{
	return OrdToBreak;
}
int Cook::getBD()
{
	return BreakDuration;
}

void Cook::setInBreak(bool in)
{
	InBreak = in;
}
bool Cook::getInBreak()
{
	return InBreak;
}