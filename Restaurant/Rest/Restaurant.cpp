#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include "..\Events\CancellationEvent.h"
#include "..\Events\PromotionEvent.h"
#include <cstdlib>
#include <time.h>
#include <iostream>
using namespace std;


Restaurant::Restaurant()
{
	pGUI = NULL;
	C_count = 0;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE mode = pGUI->getGUIMode();
	ifstream in;
	ReadInputFile(in);
	int currentstep = 0;

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
	default:
		break;
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

		pE->Execute(this);          //executes the event
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;					//deallocate event object from memory
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
	/////////////////////////////////////////////////////////////////////////////////////////

	/// The next code section should be done through function "FillDrawingList()" once you
	/// decide the appropriate list type for Orders and Cooks

	//Let's add ALL Cooks to GUI::DrawingList

	for (int i = 0; i < C_count; i++)
		pGUI->AddToDrawingList(&CookList[i]);

	//Let's add ALL Ordes to GUI::DrawingList
	///////////Adding Normal Orders to GUI::DrawingList//////////
	Order* pOrd;
	count_Ord = 0;
	int size = 0;
	Order** NRM_Orders_Array = WaitingNormal.toArray(size);
	count_Ord += size;
	for (int i = 0; i < size; i++)
	{
		pOrd = NRM_Orders_Array[i];
		pGUI->AddToDrawingList(pOrd);
	}

	///////////Adding Vegan Orders to GUI::DrawingList//////////
	Order** VGN_Orders_Array = WaitingVegan.toArray(size);
	count_Ord += size;
	for (int i = 0; i < size; i++)
	{
		pOrd = VGN_Orders_Array[i];
		pGUI->AddToDrawingList(pOrd);
	}

	///////////Adding VIP Orders to GUI::DrawingList//////////
	Order** VIP_Orders_Array = WaitingVIP.toArray(size);
	count_Ord += size;
	for (int i = 0; i < size; i++)
	{
		pOrd = VIP_Orders_Array[i];
		pGUI->AddToDrawingList(pOrd);
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//print waiting numbers
	string waitingnum = to_string(count_Ord);
	//print current timestep
	string timestep = to_string(CurrentTimeStep);

	pGUI->PrintMessage("TS: " + timestep + "   waiting num : " + waitingnum);
	///////////Adding In service Normal Orders to GUI::DrawingList//////////
	Order* pServiceOrd;
	int count_ServiceOrd = 0;
	int Servicesize = 0;
	Order** ServiceNRM_Orders_Array = InServiceNRM.toArray(Servicesize);
	count_ServiceOrd += Servicesize;
	for (int i = 0; i < Servicesize; i++)
	{
		pServiceOrd = ServiceNRM_Orders_Array[i];
		pGUI->AddToDrawingList(pServiceOrd);
	}

	///////////Adding In service Vegan Orders to GUI::DrawingList//////////
	Order** ServiceVGN_Orders_Array = InServiceVGN.toArray(Servicesize);
	count_ServiceOrd += Servicesize;
	for (int i = 0; i < Servicesize; i++)
	{
		pServiceOrd = ServiceVGN_Orders_Array[i];
		pGUI->AddToDrawingList(pServiceOrd);
	}

	///////////Adding In service VIP Orders to GUI::DrawingList//////////
	Order** ServiceVIP_Orders_Array = InServiceVIP.toArray(Servicesize);
	count_ServiceOrd += Servicesize;
	for (int i = 0; i < Servicesize; i++)
	{
		pServiceOrd = ServiceVIP_Orders_Array[i];
		pGUI->AddToDrawingList(pServiceOrd);
	}

	/////////////////////////////////////////////////////////////////////////////////////////

	///////////Adding the finished Orders to GUI::DrawingList//////////
	Order* pFinishedOrd;
	int finishedsize = 0;
	int finished = 0;
	Order** Finished_Orders_Array = FinishedList.toArray(finishedsize);
	finished += finishedsize;
	for (int i = 0; i < finishedsize; i++)
	{
		pFinishedOrd = Finished_Orders_Array[i];
		pGUI->AddToDrawingList(pFinishedOrd);
	}
}

void Restaurant::ReadInputFile(ifstream& InputFile)
{
	pGUI->PrintMessage("Please enter the input text file name: ");
	string FileName = pGUI->GetString();
	FileName = FileName + ".txt";
	InputFile.open(FileName, ios::in);

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

	C_count = N + G + V;
	CookList = new Cook[C_count];
	int cID = 1;
	CookList[0].setBO(BO);
	//Adding the normal cooks the cooks array
	for (int i = 0; i < N; i++)
	{
		cID += (rand() % 15 + 1);
		CookList[i].setID(cID);
		CookList[i].setSpeed(SN);
		CookList[i].setType(TYPE_NRM);
		CookList[i].setBD(BN);

	}

	// Adding the vegan cooks the cooks array
	for (int i = N; i < C_count - V; i++)
	{
		cID += (rand() % 15 + 1);
		CookList[i].setID(cID);
		CookList[i].setSpeed(SG);
		CookList[i].setType(TYPE_VGAN);
		CookList[i].setBD(BG);
	}

	//Adding the VIP cooks the cooks array
	for (int i = N + G; i < C_count; i++)
	{
		cID += (rand() % 15 + 1);
		CookList[i].setID(cID);
		CookList[i].setSpeed(SV);
		CookList[i].setType(TYPE_VIP);
		CookList[i].setBD(BV);
	}

	//printing the cooks information
	cout << "\nCooks Information\n";
	for (int i = 0; i < C_count; i++)
		cout << "ID: " << CookList[i].GetID() << " Type: " << CookList[i].GetType() << " Speed: " << CookList[i].GetSpeed() 
		<< " Break Duration: " << CookList[i].getBD() << " Orders to break: " << CookList[i].getBO() 
		<< " Is in break? " << CookList[i].getInBreak() << "\n";
	cout << endl;

	int EvTime = 0;

	int O_id = 0;
	CancellationEvent* pCE;
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
			int OrderSize;
			double OrderCost;
			InputFile >> OrderSize >> OrderCost;
			cout << OrderSize << " " << OrderCost << endl;
			pEv = new ArrivalEvent(EvTime, O_id, (ORD_TYPE)OrderType, OrderSize, OrderCost);
			EventsQueue.enqueue(pEv);
			break;

		case 'X':
			InputFile >> EvTime;
			cout << EvTime << " ";
			InputFile >> O_id;
			cout << O_id << "\n";
			pCE = new CancellationEvent(EvTime, O_id);
			EventsQueue.enqueue(pCE);
			break;

		case 'P':
			InputFile >> EvTime;
			cout << EvTime << " ";
			InputFile >> O_id;
			cout << O_id << " ";
			int ExtMoney;
			InputFile >> ExtMoney;
			cout << ExtMoney << endl;
			pEv = new PromotionEvent(EvTime, O_id, ExtMoney);
			EventsQueue.enqueue(pEv);
			break;

		default:
			break;
		}
	}	
}

void Restaurant::Interactive_Mode()
{
	//as long as events queue is not empty yet
	while (!EventsQueue.isEmpty() || InServiceVGN.getcount() != 0 || InServiceVIP.getcount() != 0 || InServiceNRM.getcount() != 0)
	{
		//a) Executing Events at this current step 
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step

		FillDrawingList();

		//b) Picking 1 order from each type from Waiting to be InService	
		Order* pOrd;
		if (WaitingNormal.dequeue(pOrd))
		{
			pOrd->setStatus(SRV);
			InServiceNRM.InsertBeg(pOrd);
		}

		if (WaitingVegan.dequeue(pOrd))
		{
			pOrd->setStatus(SRV);
			InServiceVGN.InsertBeg(pOrd);
		}

		if (WaitingVIP.dequeue(pOrd))
		{
			pOrd->setStatus(SRV);
			pOrd->SetInServiceTime(CurrentTimeStep);
			InServiceVIP.InsertBeg(pOrd);
		}

		//c)each 5 timesteps moving order of each type from InService to Finished list
		if (CurrentTimeStep % 5 == 0)
		{
			if (InServiceNRM.getcount() !=0)
				FinishedList.enqueue(InServiceNRM.Remove());
			if (InServiceVGN.getcount() != 0)
				FinishedList.enqueue(InServiceVGN.Remove());
			if (InServiceVIP.getcount() != 0)
				FinishedList.enqueue(InServiceVIP.Remove());
		}
		
		pGUI->UpdateInterface();
		pGUI->waitForClick();
		CurrentTimeStep++;	//advance timestep
		pGUI->ResetDrawingList();
	}

	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();
}

void Restaurant::StepByStep_Mode()
{

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

		pGUI->UpdateInterface();
		Sleep(1000);
		CurrentTimeStep++;
		pGUI->ResetDrawingList();
	}


	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();
}

void  Restaurant::AddtoVIPQueue(Order* po)	//adds an order to the demo queue
{	// To Calculate The Periority Of The Order
	int p = po->GetOrderMoney() + po->GetOrderSize() + 2 * po->GetArrivalTime();
	WaitingVIP.enqueue(po, p);
}

void  Restaurant::AddtoNormalQueue(Order* po)
{
	WaitingNormal.enqueue(po);
}

void  Restaurant::AddtoVeganQueue(Order* po)
{
	WaitingVegan.enqueue(po);
}

bool  Restaurant::DeleteNormalQueue(int id)
{
	Order* OrderToDelete = WaitingNormal.SearchForOrder(id);
	if (OrderToDelete)
	{
		WaitingNormal.dequeue(OrderToDelete);
		return true;
	}
	return false;
}