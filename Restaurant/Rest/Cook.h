#pragma once
#include "..\Defs.h"

class Cook
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)
	int BreakDuration;
	int CurrOrd;
	bool InBreak;
	static int OrdToBreak;
public:
	Cook();
	virtual ~Cook();
	int GetID() const;
	ORD_TYPE GetType() const;
	int GetSpeed() const;
	void setID(int);
	void setType(ORD_TYPE);
	void setSpeed(int);
	static void setBO(int);
	void setBD(int);
	static int getBO();
	int getBD();
	void setInBreak(bool);
	bool getInBreak();
	//e3mly func lel availability
};