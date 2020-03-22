#pragma once

#include <fstream>
#include "Order.h"
#include "Cook.h"
#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include"..\Generic_DS\PriorityQueue.h"
#include "..\Generic_DS\LinkedList.h"

// it is the maestro of the project
class Event;

class Restaurant  
{	
private:
	GUI *pGUI;
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file
	Queue<Order*> WaitingNormal;
	Queue<Order*> WaitingVegan;
	PriorityQueue<Order*> WaitingVIP;
	Queue<Order*>FinishedList;
	LinkedList<Order*> InServiceNRM;
	LinkedList<Order*> InServiceVIP;
	LinkedList<Order*> InServiceVGN;
	Cook* CookList;
	/// ==> 
	//	DEMO-related members. Should be removed in phases 1&2
	Queue<Order*> DEMO_Queue;	//Important: This is just for demo
	/// ==>
	
	
	
	//
	// TODO: Add More Data Members As Needed
	//

public:
	
	Restaurant();
	~Restaurant();
	
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();

	


	void FillDrawingList();

	//
	// TODO: Add More Member Functions As Needed
	//
	void Interactive_Mode();
	void StepByStep_Mode();
	void ReadInputFile(ifstream & InputFile);
	void AddtoVIPQueue(Order* po);	//adds an order to the demo queue
	void AddtoNormalQueue(Order* po);
	void AddtoVeganQueue(Order* po);
	bool DeleteNormalQueue(int id);


/// ================================================================================================== 



};