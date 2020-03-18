#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"


Restaurant::Restaurant()
{
	pGUI = NULL;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();

	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		Interactive_Mode();
		break;
	case MODE_STEP:
		StepByStep_Mode();
		break;
	case MODE_SLNT:
		break;
	case MODE_DEMO:
		Just_A_Demo();

	};

}



//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event* pE;
	while (EventsQueue.peekFront(pE))	//as long as there are more events
	{
		if (pE->getEventTime() > CurrentTimeStep)	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}


Restaurant::~Restaurant()
{
	if (pGUI)
		delete pGUI;
}

void Restaurant::FillDrawingList()
{
	//This function should be implemented in phase1
	//It should add ALL orders and Cooks to the drawing list
	//It should get orders from orders lists/queues/stacks/whatever (same for Cooks)
	//To add orders it should call function  void GUI::AddToDrawingList(Order* pOrd);
	//To add Cooks it should call function  void GUI::AddToDrawingList(Cook* pCc);

}

void Restaurant::Interactive_Mode()
{
	pGUI->PrintMessage("Please enter the input text file name: ");
	string FileName = pGUI->GetString();
	ifstream InputFile(FileName, ios::in);

	int N = 0, G = 0, V = 0;
	int SN = 0, SG = 0, SV = 0;
	int BO = 0, BN = 0, BG = 0, BV = 0;
	int AutoP = 0, M = 0;
	InputFile >> N >> G >> V;
	cout << N << " " << G << " " << V << endl;
	InputFile >> SN >> SG >> SV;
	cout << SN << " " << SG << " " << SV << endl;
	InputFile >> BO >> BN >> BG >> BV;
	cout << BO << " " << BN << " " << BG << " " << BV << endl;
	InputFile >> AutoP;
	cout << AutoP << endl;
	InputFile >> M;
	cout << M << endl;

	int EventCnt = M;
	Order* pOrd;
	Event* pEv;
	srand(time(NULL));

	pGUI->PrintMessage("CLICK to continue ...");
	pGUI->waitForClick();

	int C_count = N + G + V;
	Cook* pC = new Cook[C_count];
	int cID = 1;

	//Adding the normal cooks the cooks array
	for (int i = 0; i < N; i++)
	{
		cID += (rand() % 15 + 1);
		pC[i].setID(cID);
		pC[i].setSpeed(SN);
		pC[i].setType(TYPE_NRM);
	}

	// Adding the vegan cooks the cooks array
	for (int i = N; i < C_count - V; i++)
	{
		cID += (rand() % 15 + 1);
		pC[i].setID(cID);
		pC[i].setSpeed(SG);
		pC[i].setType(TYPE_VGAN);
	}

	//Adding the VIP cooks the cooks array
	for (int i = N + G; i < C_count; i++)
	{
		cID += (rand() % 15 + 1);
		pC[i].setID(cID);
		pC[i].setSpeed(SV);
		pC[i].setType(TYPE_VIP);
	}

	//printing the cooks information
	for (int i = 0; i < C_count; i++)
		cout << pC[i].GetID() << " " << pC[i].GetSpeed() << " " << pC[i].GetType() << "\n";

	int EvTime = 0;

	int O_id = 0;

	for (int i = 0; i < EventCnt; i++)
	{
		char EventType = ' ';
		InputFile >> EventType;
		cout << EventType << " ";

		switch (EventType)
		{
		case 'R':
			char OType;
			InputFile >> OType;
			cout << OType << " ";
			int OrderType;                                          //
			(OType == 'N') ? OrderType = 0 :                        //These 3 lines convert the order type from char (N, G, V) to ORD_TYPE
				(OType == 'G') ? OrderType = 1 : OrderType = 2;     //
			InputFile >> EvTime;
			cout << EvTime << " ";
			InputFile >> O_id;
			cout << O_id << " ";
			int OrderSize, OrderCost;
			InputFile >> OrderSize >> OrderCost;
			cout << OrderSize << " " << OrderCost << endl;
			pEv = new ArrivalEvent(EvTime, O_id, (ORD_TYPE)OrderType);
			EventsQueue.enqueue(pEv);

			break;

		case 'X':
			InputFile >> EvTime;
			cout << EvTime << " ";
			InputFile >> O_id;
			cout << O_id << "\n";

			break;

		case 'P':
			InputFile >> EvTime;
			cout << EvTime << " ";
			InputFile >> O_id;
			cout << O_id << " ";
			int ExtMoney;
			InputFile >> ExtMoney;
			cout << ExtMoney << endl;

			break;

		default:
			break;
		}
	}
	//Now We have filled EventsQueue

	int CurrentTimeStep = 1;


	//as long as events queue is not empty yet
	while (!EventsQueue.isEmpty())
	{
		//print current timestep
		char timestep[10];
		itoa(CurrentTimeStep, timestep, 10);
		pGUI->PrintMessage(timestep);


		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step


/////////////////////////////////////////////////////////////////////////////////////////
		/// The next code section should be done through function "FillDrawingList()" once you
		/// decide the appropriate list type for Orders and Cooks

		//Let's add ALL Cooks to GUI::DrawingList
		for (int i = 0; i < C_count; i++)
			pGUI->AddToDrawingList(&pC[i]);

		//Let's add ALL Ordes to GUI::DrawingList
		int size = 0;
		Order** Demo_Orders_Array = DEMO_Queue.toArray(size);

		for (int i = 0; i < size; i++)
		{
			pOrd = Demo_Orders_Array[i];
			pGUI->AddToDrawingList(pOrd);
		}
		/////////////////////////////////////////////////////////////////////////////////////////

		pGUI->UpdateInterface();
		pGUI->waitForClick();
		CurrentTimeStep++;	//advance timestep
		pGUI->ResetDrawingList();
	}

	delete[]pC;


	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();
}

void Restaurant::StepByStep_Mode()
{
	pGUI->PrintMessage("Please enter the input text file name: ");
	string FileName = pGUI->GetString();
	ifstream InputFile;
	FileName = FileName + ".txt";
	InputFile.open(FileName);
	if (!(InputFile.is_open()))
	{
		pGUI->PrintMessage("this file is not found ,please try again ...");

	}


	int N = 0, G = 0, V = 0;
	int SN = 0, SG = 0, SV = 0;
	int BO = 0, BN = 0, BG = 0, BV = 0;
	int AutoP = 0, M = 0;
	InputFile >> N >> G >> V;
	InputFile >> SN >> SG >> SV;
	InputFile >> BO >> BN >> BG >> BV;
	InputFile >> AutoP;
	InputFile >> M;
	int EventCnt = M;
	Order* pOrd;
	Event* pEv;
	srand(time(NULL));

	pGUI->PrintMessage("CLICK to continue ...");
	pGUI->waitForClick();

	int C_count = N + G + V;
	Cook* pC = new Cook[C_count];
	int cID = 1;

	//Adding the normal cooks the cooks array
	for (int i = 0; i < N; i++)
	{
		cID += (rand() % 15 + 1);
		pC[i].setID(cID);
		pC[i].setSpeed(SN);
		pC[i].setType(TYPE_NRM);
	}

	// Adding the vegan cooks the cooks array
	for (int i = N; i < C_count - V; i++)
	{
		cID += (rand() % 15 + 1);
		pC[i].setID(cID);
		pC[i].setSpeed(SG);
		pC[i].setType(TYPE_VGAN);
	}

	//Adding the VIP cooks the cooks array
	for (int i = N + G; i < C_count; i++)
	{
		cID += (rand() % 15 + 1);
		pC[i].setID(cID);
		pC[i].setSpeed(SV);
		pC[i].setType(TYPE_VIP);
	}

	//printing the cooks information
	for (int i = 0; i < C_count; i++)
		cout << pC[i].GetID() << " " << pC[i].GetSpeed() << " " << pC[i].GetType() << "\n";

	int EvTime = 0;

	int O_id = 0;

	for (int i = 0; i < EventCnt; i++)
	{
		char EventType = ' ';
		InputFile >> EventType;
		cout << EventType << " ";

		switch (EventType)
		{
		case 'R':
			char OType;
			InputFile >> OType;
			cout << OType << " ";
			int OrderType;                                          //
			(OType == 'N') ? OrderType = 0 :                        //These 3 lines convert the order type from char (N, G, V) to ORD_TYPE
				(OType == 'G') ? OrderType = 1 : OrderType = 2;     //
			InputFile >> EvTime;
			cout << EvTime << " ";
			InputFile >> O_id;
			cout << O_id << " ";
			int OrderSize, OrderCost;
			InputFile >> OrderSize >> OrderCost;
			cout << OrderSize << " " << OrderCost << endl;
			pEv = new ArrivalEvent(EvTime, O_id, (ORD_TYPE)OrderType);
			EventsQueue.enqueue(pEv);

			break;

		case 'X':
			InputFile >> EvTime;
			cout << EvTime << " ";
			InputFile >> O_id;
			cout << O_id << "\n";

			break;

		case 'P':
			InputFile >> EvTime;
			cout << EvTime << " ";
			InputFile >> O_id;
			cout << O_id << " ";
			int ExtMoney;
			InputFile >> ExtMoney;
			cout << ExtMoney << endl;

			break;

		default:
			break;
		}
	}
	//Now We have filled EventsQueue

	int CurrentTimeStep = 1;


	//as long as events queue is not empty yet
	while (!EventsQueue.isEmpty())
	{
		//print current timestep
		char timestep[10];
		itoa(CurrentTimeStep, timestep, 10);
		pGUI->PrintMessage(timestep);


		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step


/////////////////////////////////////////////////////////////////////////////////////////
		/// The next code section should be done through function "FillDrawingList()" once you
		/// decide the appropriate list type for Orders and Cooks

		//Let's add ALL Cooks to GUI::DrawingList
		for (int i = 0; i < C_count; i++)
			pGUI->AddToDrawingList(&pC[i]);

		//Let's add ALL Ordes to GUI::DrawingList
		int size = 0;
		Order** Demo_Orders_Array = DEMO_Queue.toArray(size);

		for (int i = 0; i < size; i++)
		{
			pOrd = Demo_Orders_Array[i];
			pGUI->AddToDrawingList(pOrd);
		}
		/////////////////////////////////////////////////////////////////////////////////////////

		pGUI->UpdateInterface();
		CurrentTimeStep++;	//advance timestep
		pGUI->ResetDrawingList();
	}

	delete[]pC;


	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();
}


//////////////////////////////////////////////////////////////////////////////////////////////
/// ==> 
///  DEMO-related functions. Should be removed in phases 1&2

//Begin of DEMO-related functions

//This is just a demo function for project introductory phase
//It should be removed starting phase 1
void Restaurant::Just_A_Demo()
{

	//
	// THIS IS JUST A DEMO FUNCTION
	// IT SHOULD BE REMOVED IN PHASE 1 AND PHASE 2

	int EventCnt;
	Order* pOrd;
	Event* pEv;
	srand(time(NULL));

	pGUI->PrintMessage("Just a Demo. Enter EVENTS Count(next phases should read I/P filename):");
	EventCnt = atoi(pGUI->GetString().c_str());	//get user input as a string then convert to integer

	pGUI->PrintMessage("Generating Events randomly... In next phases, Events should be loaded from a file...CLICK to continue");
	pGUI->waitForClick();

	//Just for sake of demo, generate some cooks and add them to the drawing list
	//In next phases, Cooks info should be loaded from input file
	
	int C_count = 5;
	Cook* pC = new Cook[C_count];
	int cID = 1;

	for(int i=0; i<C_count; i++)
	{
		cID+= (rand()%15+1);
		pC[i].setID(cID);
		pC[i].setType((ORD_TYPE)(rand()%TYPE_CNT));
	}

	int EvTime = 0;

	int O_id = 1;

	//Create Random events and fill them into EventsQueue
	//All generated event will be "ArrivalEvents" for the demo
	for (int i = 0; i < EventCnt; i++)
	{
		O_id += (rand() % 4 + 1);
		int OType = rand() % TYPE_CNT;	//Randomize order type		
		EvTime += (rand() % 5 + 1);			//Randomize event time
		pEv = new ArrivalEvent(EvTime, O_id, (ORD_TYPE)OType);
		EventsQueue.enqueue(pEv);

	}

	// --->   In next phases, no random generation is done
	// --->   EventsQueue should be filled from actual events loaded from input file





	//Now We have filled EventsQueue (randomly)
	int CurrentTimeStep = 1;


	//as long as events queue is not empty yet
	while (!EventsQueue.isEmpty())
	{
		//print current timestep
		char timestep[10];
		itoa(CurrentTimeStep, timestep, 10);
		pGUI->PrintMessage(timestep);


		//The next line may add new orders to the DEMO_Queue
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step


/////////////////////////////////////////////////////////////////////////////////////////
		/// The next code section should be done through function "FillDrawingList()" once you
		/// decide the appropriate list type for Orders and Cooks

		//Let's add ALL randomly generated Cooks to GUI::DrawingList
		for (int i = 0; i < C_count; i++)
			pGUI->AddToDrawingList(&pC[i]);

		//Let's add ALL randomly generated Ordes to GUI::DrawingList
		int size = 0;
		Order** Demo_Orders_Array = DEMO_Queue.toArray(size);

		for (int i = 0; i < size; i++)
		{
			pOrd = Demo_Orders_Array[i];
			pGUI->AddToDrawingList(pOrd);
		}
		/////////////////////////////////////////////////////////////////////////////////////////

		pGUI->UpdateInterface();
		Sleep(1000);
		CurrentTimeStep++;	//advance timestep
		pGUI->ResetDrawingList();
	}

	delete[]pC;


	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();


}
////////////////

void Restaurant::AddtoDemoQueue(Order* pOrd)
{
	DEMO_Queue.enqueue(pOrd);
}

/// ==> end of DEMO-related function
//////////////////////////////////////////////////////////////////////////////////////////////


