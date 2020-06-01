#pragma once
#include <fstream>
#include "Order.h"
#include "Cook.h"
#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Generic_DS\PriorityQueue.h"
#include "..\Generic_DS\List.h"

//The maestro of the project
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
	List<Cook*> WorkingCooks;			//List of working cooks of all types
	List<Cook*> InBreakCooks;			//List of unavailable cooks of all types
	List<Cook*> InjuredCooks;			//List of injured cooks of all types
	int C_count;			//No. of total cooks
	int NRM_C;				//No. of available normal cooks
	int VGN_C;				//No. of available vegan cooks 
	int VIP_C;				//No. of available vip cooks
	int AutoPromoted;		//No. of auto promoted orders
	int UrgentOrders;		//No. of urgent orders
	int OriginalNRMOrders;	//Original number of normal orders
	int NRM_OrdCount, VGN_OrdCount, VIP_OrdCount;					//No. of waiting orders
	int NRM_SRVCount, VGN_SRVCount, VIP_SRVCount;					//No. of served orders
	int NRM_FinishedCount, VGN_FinishedCount, VIP_FinishedCount;	//No. of finished orders
	int CurrentTimeStep;

public:

	Restaurant();
	~Restaurant();

	void ExecuteEvents(int TimeStep);			//executes all events at current timestep
	void RunSimulation();						//It calls the functions needed to simulate the restaurant system
	void FillDrawingList();						//Adds ALL ordersand Cooks to the drawing list
	void ReadInputFile(ifstream& InputFile);	//Read the data from the input file and fills it in the EventsQueue
	void WritingOutputFile();					//Writes the final statistics of the simulation in the output file
	void AddtoVIPQueue(Order* po);				//Adds an order to the VIP Queue
	void AddtoNormalQueue(Order* po);			//Adds an order to the Normal Queue
	void AddtoVeganQueue(Order* po);			//Adds an order to the Vegan Queue
	bool DeletefromNormalQueue(int id);			//Deletes an order from the Normal Queue using the passed ID
	Order*& PromotOrder(int id, double Extra);	//Promotes an order with its ID from Normal to VIP by an Extra amount of money
	void PrintData();							//Prints the data in the status bar in GUI at each Timestep
	void UpdateCooks();							//Updates the cooks at each Timestep
	void UpdateInService();						//Updats In-Service orders at each Timestep
	void UpdateWaiting();						//Updats Waiting Orders at each Timestep
	void Assigning();							//Assign the orders to the required cook according to the Orders Service Criteria 
};