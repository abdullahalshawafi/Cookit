 #pragma once
#include "..\Defs.h"
#include "Order.h"

class Order;

class Cook
{
	int ID;				    //Each cook has its own ID
	ORD_TYPE type;		    //for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;			    //dishes it can prepare in one clock tick (in one timestep)
	int BreakDuration;      //No. of time steps that a cook stay in break
	int CurrOrd;		    //No. of the orders that have been assigned to the cook
	bool Available;			//To Now If The Cook Is Available OR Not
	int TimeToBeFree;		//Time When The cook Will Be Free
	bool InBreak;			//Is the cook in a break or not
	Order* AssignedOrder;   //Pointer to the assigned order
	static int OrdToBreak;	//How many orders does a cook serve before a break
	int BreakTimestep;		//The timestep at which cook had break
	int originalspeed;      // original speed before the cook injured
	int ToBackFromRest;     //time when the cook should back from rest
	bool Injured;
	static float injurypro;
	static int restperiod;
public:
	Cook();
	
	void SetID(int);
	int GetID() const;

	void SetType(ORD_TYPE);
	ORD_TYPE GetType() const;

	void SetSpeed(int);
	int GetSpeed() const;

	void SetBreakDuration(int);
	int GetBreakDuration() const;

	void SetCurrOrd(int);
	int GetCurrOrd() const;

	void SetAvailable(bool);
	bool GetAvailable();

	void SetTimeToBeFree(int);
	int GetTimeToBeFree();

	void SetInBreak(bool);
	bool GetInBreak() const;

	void SetAssignedOrder(Order*);
	Order* GetAssignedOrder() const;

	static void SetOrdToBreakANDRest(int ,int); //first parameter for OrderToBreak ,second for Rest duration 
	static int GetOrdToBreakANDRest(int &);		//returns OrderToBreak ,and sets the parameter by Rest duration

	void SetBreakTS(int TS);
	int GetBreakTS();

	void SetToBackFromRest(int);
	int GetToBackFromRest();

	void SetInjured(bool);
	bool GetInjured();

	void SetOriginalSpeed(int);
	void ResetSpeed();

	static void SetInjProp(float);
	static float GetInjProp();

	virtual ~Cook();
};