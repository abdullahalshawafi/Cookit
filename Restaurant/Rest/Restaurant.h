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
	GUI* pGUI;
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file
	Queue<Order*> WaitingNormal;
	Queue<Order*> WaitingVegan;
	PriorityQueue<Order*> WaitingVIP;
	Queue<Order*>FinishedList;
	LinkedList<Order*> InServiceNRM;
	LinkedList<Order*> InServiceVIP;
	LinkedList<Order*> InServiceVGN;
	int C_count;
	int count_Ord;
	int CurrentTimeStep = 1;
	Cook* CookList;
public:

	Restaurant();
	~Restaurant();

	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();
	void FillDrawingList();
	void Interactive_Mode();
	void StepByStep_Mode();
	void ReadInputFile(ifstream& InputFile);
	void AddtoVIPQueue(Order* po);
	void AddtoNormalQueue(Order* po);
	void AddtoVeganQueue(Order* po);
	bool DeleteNormalQueue(int id);
};