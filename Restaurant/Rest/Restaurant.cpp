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
	NRM_OrdCount = 0;
	VGN_OrdCount = 0;
	VIP_OrdCount = 0;
	NRM_SRVCount = 0;
	VGN_SRVCount = 0;
	VIP_SRVCount = 0;
	NRM_FinishedCount = 0;
	VGN_FinishedCount = 0;
	VIP_FinishedCount = 0;
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
	int C_size = 0;
	Cook** CookArr = AvailableCooks.toArray(C_size);
	for (int i = 0; i < C_size; i++)
	{
		pGUI->AddToDrawingList(CookArr[i]);
	}

	//Let's add ALL Ordes to GUI::DrawingList

	///////////Adding Normal Orders to GUI::DrawingList//////////
	Order* pOrd;
	NRM_OrdCount = 0;
	int size = 0;
	Order** NRM_Orders_Array = WaitingNormal.toArray(size);
	NRM_OrdCount += size;
	for (int i = 0; i < size; i++)
	{
		pOrd = NRM_Orders_Array[i];
		pGUI->AddToDrawingList(pOrd);
	}

	///////////Adding Vegan Orders to GUI::DrawingList//////////
	VGN_OrdCount = 0;
	Order** VGN_Orders_Array = WaitingVegan.toArray(size);
	VGN_OrdCount += size;
	for (int i = 0; i < size; i++)
	{
		pOrd = VGN_Orders_Array[i];
		pGUI->AddToDrawingList(pOrd);
	}

	///////////Adding VIP Orders to GUI::DrawingList//////////
	VIP_OrdCount = 0;
	Order** VIP_Orders_Array = WaitingVIP.toArray(size);
	VIP_OrdCount += size;
	for (int i = 0; i < size; i++)
	{
		pOrd = VIP_Orders_Array[i];
		pGUI->AddToDrawingList(pOrd);
	}

	///////////Adding In service Normal Orders to GUI::DrawingList//////////
	Order* pServiceOrd;
	NRM_SRVCount = 0;
	int Servicesize = 0;
	Order** ServiceNRM_Orders_Array = InServiceNRM.toArray(Servicesize);
	NRM_SRVCount += Servicesize;
	for (int i = 0; i < Servicesize; i++)
	{
		pServiceOrd = ServiceNRM_Orders_Array[i];
		pGUI->AddToDrawingList(pServiceOrd);
	}

	///////////Adding In service Vegan Orders to GUI::DrawingList//////////
	VGN_SRVCount = 0;
	Order** ServiceVGN_Orders_Array = InServiceVGN.toArray(Servicesize);
	VGN_SRVCount += Servicesize;
	for (int i = 0; i < Servicesize; i++)
	{
		pServiceOrd = ServiceVGN_Orders_Array[i];
		pGUI->AddToDrawingList(pServiceOrd);
	}

	///////////Adding In service VIP Orders to GUI::DrawingList//////////
	VIP_SRVCount = 0;
	Order** ServiceVIP_Orders_Array = InServiceVIP.toArray(Servicesize);
	VIP_SRVCount += Servicesize;
	for (int i = 0; i < Servicesize; i++)
	{
		pServiceOrd = ServiceVIP_Orders_Array[i];
		pGUI->AddToDrawingList(pServiceOrd);
	}

	/////////////////////////////////////////////////////////////////////////////////////////

	///////////Adding the finished Normal Orders to GUI::DrawingList//////////
	Order* pFinishedOrd;
	NRM_FinishedCount = 0;
	int finishedsize = 0;
	Order** FinishedNRM_Orders_Array = FinishedNRM.toArray(finishedsize);
	NRM_FinishedCount += finishedsize;
	for (int i = 0; i < finishedsize; i++)
	{
		pFinishedOrd = FinishedNRM_Orders_Array[i];
		pGUI->AddToDrawingList(pFinishedOrd);
	}

	///////////Adding the finished Vegan Orders to GUI::DrawingList//////////
	VGN_FinishedCount = 0;
	Order** FinishedVGN_Orders_Array = FinishedVGN.toArray(finishedsize);
	VGN_FinishedCount += finishedsize;
	for (int i = 0; i < finishedsize; i++)
	{
		pFinishedOrd = FinishedVGN_Orders_Array[i];
		pGUI->AddToDrawingList(pFinishedOrd);
	}

	///////////Adding the finished Normal Orders to GUI::DrawingList//////////
	VIP_FinishedCount = 0;
	Order** FinishedVIP_Orders_Array = FinishedVIP.toArray(finishedsize);
	VIP_FinishedCount += finishedsize;
	for (int i = 0; i < finishedsize; i++)
	{
		pFinishedOrd = FinishedVIP_Orders_Array[i];
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
	Cook* pCook;

	//Adding the normal cooks the cooks array
	for (int i = 0; i < N; i++)
	{
		pCook = new Cook;
		pCook->setID(i + 1);
		pCook->setSpeed(SN);
		pCook->setType(TYPE_NRM);
		pCook->setBD(BN);
		pCook->setBO(BO);
		AvailableCooks.InsertEnd(pCook);
	}
	// Adding the vegan cooks the cooks array
	for (int i = N; i < C_count - V; i++)
	{
		pCook = new Cook;
		pCook->setID(i + 1);
		pCook->setSpeed(SN);
		pCook->setType(TYPE_VGAN);
		pCook->setBD(BN);
		pCook->setBO(BO);
		AvailableCooks.InsertEnd(pCook);
	}
	//Adding the VIP cooks the cooks array
	for (int i = N + G; i < C_count; i++)
	{
		pCook = new Cook;
		pCook->setID(i + 1);
		pCook->setSpeed(SN);
		pCook->setType(TYPE_VIP);
		pCook->setBD(BN);
		pCook->setBO(BO);
		AvailableCooks.InsertEnd(pCook);
	}

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
	bool notFinished = false;
	do	//as long as events queue is not empty yet
	{
		//a) Executing Events at this current step 
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step

		FillDrawingList();

		/////////////////////////////////////////////////////////////////////////////////////////

		string waitingNRM = to_string(NRM_OrdCount);
		string waitingVGN = to_string(VGN_OrdCount);
		string waitingVIP = to_string(VIP_OrdCount);

		string finishedNRM = to_string(NRM_FinishedCount);
		string finishedVGN = to_string(VGN_FinishedCount);
		string finishedVIP = to_string(VIP_FinishedCount);

		string timestep = to_string(CurrentTimeStep);

		pGUI->PrintMessage("TS: " + timestep);	//print current timestep
		pGUI->PrintWaitingOrders("Waiting: (Normal = " + waitingNRM + "), (Vegan = " + waitingVGN + "), (VIP = " + waitingVIP + ")");	//print waiting orders numbers
		pGUI->PrintAvailableCooks("Cooks: (Normal = 5), (Vegan = 3), (VIP = 1)");
		pGUI->PrintAssignedOrders("N6(V3)");
		pGUI->PrintFinishedOrders("Finished: (Normal = " + finishedNRM + "), (Vegan = " + finishedVGN + "), (VIP = " + finishedVIP + ")");	//print finished orders numbers

		//b) Picking 1 order from each type from Waiting to be InService	
		Order* pOrd;
		Order* pOrd1;
		int csize = 0, i = 0;
		Cook** C_Arr = AvailableCooks.toArray(csize);

		if (WaitingNormal.dequeue(pOrd))
		{
			pOrd->setStatus(SRV);
			pOrd->SetInServiceTime(CurrentTimeStep);
			InServiceNRM.InsertBeg(pOrd);
			AvailableCooks.InsertEnd(AvailableCooks.RemoveFirst()->getItem());
			/*C_Arr[i]->setCurrOrd(C_Arr[i]->getCurrOrd() + 1);
			i++;*/
		}

		if (WaitingVegan.dequeue(pOrd))
		{
			pOrd->setStatus(SRV);
			pOrd->SetInServiceTime(CurrentTimeStep);
			InServiceVGN.InsertBeg(pOrd);
			AvailableCooks.InsertEnd(AvailableCooks.RemoveFirst()->getItem());
			/*C_Arr[i]->setCurrOrd(C_Arr[i]->getCurrOrd() + 1);
			i++;*/
		}

		if (WaitingVIP.dequeue(pOrd))
		{
			pOrd->setStatus(SRV);
			pOrd->SetInServiceTime(CurrentTimeStep);
			InServiceVIP.InsertBeg(pOrd);
			AvailableCooks.InsertEnd(AvailableCooks.RemoveFirst()->getItem());
			/*C_Arr[i]->setCurrOrd(C_Arr[i]->getCurrOrd() + 1);
			i++;*/
		}

		//c)each 5 timesteps moving order of each type from InService to Finished list
		if ((CurrentTimeStep + 1) % 5 == 0)
		{
			if (InServiceNRM.getcount() != 0)
			{
				pOrd1 = InServiceNRM.Remove();
				if (pOrd1->GetArrivalTime() == pOrd1->getInServiceTime())
					InServiceNRM.InsertEnd(pOrd1);
				else
				{
					pOrd1->setStatus(DONE);
					FinishedNRM.enqueue(pOrd1);
				}
				/*else
					InServiceNRM.InsertBeg(pOrd1);*/
			}

			if (InServiceVGN.getcount() != 0)
			{
				pOrd1 = InServiceVGN.Remove();
				if (pOrd1->GetArrivalTime() != pOrd1->getInServiceTime())
				{
					pOrd1->setStatus(DONE);
					FinishedVGN.enqueue(pOrd1);
				}
				else
					InServiceVGN.InsertEnd(pOrd1);
				/*else
					InServiceVGN.InsertBeg(pOrd1);*/
			}

			if (InServiceVIP.getcount() != 0)
			{
				pOrd1 = InServiceVIP.Remove();
				if (pOrd1->GetArrivalTime() != pOrd1->getInServiceTime())
				{
					pOrd1->setStatus(DONE);
					FinishedVIP.enqueue(pOrd1);
				}
				else
					InServiceVIP.InsertEnd(pOrd1);
				///*else
				//	InServiceVIP.InsertBeg(pOrd1);
			}
		}

		notFinished = !EventsQueue.isEmpty() || InServiceVGN.getcount() != 0 || InServiceVIP.getcount() != 0 || InServiceNRM.getcount() != 0 || !WaitingNormal.isEmpty() || !WaitingVegan.isEmpty() || !WaitingVIP.isEmpty();

		pGUI->UpdateInterface();
		pGUI->ResetDrawingList();
		pGUI->waitForClick();
		CurrentTimeStep++;	//advance timestep

	} while (notFinished);

	FillDrawingList();
	pGUI->UpdateInterface();
	pGUI->PrintMessageWithoutClearing("generation done, click to END program");
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
	bool IsFound = WaitingNormal.SearchForOrder(id);
	return IsFound;
}