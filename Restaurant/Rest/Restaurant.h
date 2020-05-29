#pragma once

#include <fstream>
#include "Order.h"
#include "Cook.h"
#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include"..\Generic_DS\PriorityQueue.h"
#include "..\Generic_DS\List.h"

// it is the maestro of the project
class Event;

class Restaurant
{
	GUI* pGUI;
	Queue<Event*> EventsQueue;			//Queue of all events that will be loaded from file
	Queue<Order*> WaitingNormal;		//Queue of all waiting orders of type normal
	Queue<Order*> WaitingVegan;			//Queue of all waiting orders of type Vegan
	PriorityQueue<Order*> WaitingVIP;   //Queue of all waiting orders of type VIP
	Queue<Order*>Finished;				//Queue of all Finished orders
	List<Order*> InService;				//List of all In-service orders
	List<Cook*> AVAILABLECOOKS;			//List of available cooks of all types
	List<Cook*> InBreakCooks;			//List of unavailable cooks of all types
	List<Cook*> InjuredCooks;			//List of injured cooks of all types
	int C_count;		//No. of total cooks
	int NRM_C;			//No. of available normal cooks
	int VGN_C;			//No. of available vegan cooks 
	int VIP_C;			//No. of available vip cooks
	int AutoPromoted;	//No. of auto promoted orders
	int UrgentOrders;	//No. of urgent orders
	int NRM_OrdCount, VGN_OrdCount, VIP_OrdCount;					//No. of waiting orders
	int NRM_SRVCount, VGN_SRVCount, VIP_SRVCount;					//No. of served orders
	int NRM_FinishedCount, VGN_FinishedCount, VIP_FinishedCount;	//No. of finished orders
	int CurrentTimeStep;

public:

	Restaurant();
	~Restaurant();

	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();
	void FillDrawingList();
	void ReadInputFile(ifstream& InputFile);
	void WritingOutputFile();
	void AddtoVIPQueue(Order* po);
	void AddtoNormalQueue(Order* po);
	void AddtoVeganQueue(Order* po);
	bool DeleteNormalQueue(int id);
	Order*& PromotOrder(int id, double Extra);
	void PrintData();
	void UpdateCooks();
	void UpdateInServiceOrders();
	void UpdateWaiting();
	void Assigning();
};