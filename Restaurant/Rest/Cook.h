#pragma once
#include "..\Defs.h"
#include "Order.h"

class Order;

class Cook
{
	int ID;				    //Each cook has its own ID
	ORD_TYPE type;		    //For each order type there is a corresponding cook type (VIP, Normal, Vegan)
	int speed;			    //Dishes that a cook can prepare in one clock tick (in one timestep)
	int BreakDuration;      //No. of timesteps that a cook stay in break
	int FinishedOrders;		//No. of the orders that have been finished by the cook
	int BreakTimestep;		//The timestep at which cook went to break
	int OriginalSpeed;      //Original speed before the cook got injured
	int BackFromRest;		//Timestep when the cook should get back from rest
	bool InBreak;			//Is the cook in a break or not
	bool Injured;			//Is the cook injured of not
	Order* AssignedOrder;   //Pointer to the assigned order
	static int OrdToBreak;	//How many orders does a cook serve before a break
	static int RestPeriod;	//No. of timesteps that a cook stay in rest
	static float InjuryPro;	//The probability of a cook getting injured
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

	void SetFinishedOrders(int);
	int GetFinishedOrders() const;

	void SetBreakTS(int TS);
	int GetBreakTS();

	void SetOriginalSpeed(int);
	void ResetSpeed();

	void SetBackFromRest(int);
	int GetBackFromRest();

	//void SetTimeToBeFree(int);
	//int GetTimeToBeFree();

	/*void SetAvailable(bool);
	bool GetAvailable();*/

	void SetInBreak(bool);
	bool GetInBreak() const;

	void SetInjured(bool);
	bool GetInjured();

	void SetAssignedOrder(Order*);
	Order* GetAssignedOrder() const;

	static void SetOrdToBreakANDRest(int, int); //first parameter for OrderToBreak, second for Rest duration 
	static int GetOrdToBreakANDRest(int&);		//returns OrderToBreak, and sets the parameter by Rest duration

	static void SetInjProp(float);
	static float GetInjProp();

	virtual ~Cook();
};