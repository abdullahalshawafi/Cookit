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
	TotalNRMOrders = 0;
	NRM_FinishedCount = 0;
	VGN_FinishedCount = 0;
	VIP_FinishedCount = 0;
	NRM_C = 0;
	VIP_C = 0;
	VGN_C = 0;
	injured = 0;
	AutoPromoted = 0;
	UrgentOrders = 0;
	CurrentTimeStep = 1;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE mode = pGUI->getGUIMode();
	ifstream inputFile;
	ReadInputFile(inputFile);

	bool notFinished = true;

	while (notFinished)	//as long as events queue is not empty yet
	{
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step
		UpdateInServiceOrders();
		UpdateCooks();
		Assigning();
		UpdateWaiting();
		FillDrawingList();
		if (mode != MODE_SLNT)
		{
			pGUI->UpdateInterface();
			PrintData();
		}
		pGUI->ResetDrawingList();
		if (mode == MODE_INTR)
			pGUI->waitForClick();
		else if (mode == MODE_STEP)
			Sleep(1000);
		CurrentTimeStep++;	//advance timestep
		notFinished = !EventsQueue.isEmpty() || InService.GetCount() != 0 || !WaitingNormal.isEmpty() || !WaitingVegan.isEmpty() || !WaitingVIP.isEmpty() || !InBreakCooks.IsEmpty() || !InjuredCooks.IsEmpty();
	}
	if (mode == MODE_SLNT)
		pGUI->PrintMessage("generation done, click to save the information in the output text file.", 1, 0);
	else
		pGUI->PrintMessage("generation done, click to save the information in the output text file.", 6, 0);
	pGUI->waitForClick();
	WritingOutputFile();
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

//This function should add ALL orders and Cooks to the drawing list
//It should get orders from orders lists/queues/stacks/whatever (same for Cooks)
void Restaurant::FillDrawingList()
{
	//Let's add ALL Cooks to GUI::DrawingList
	int C_size = 0;
	VIP_C = 0;
	VGN_C = 0;
	NRM_C = 0;
	Cook** CookArr = AVAILABLECOOKS.toArray(C_size);
	for (int i = 0; i < C_size; i++)
	{
		switch (CookArr[i]->GetType())
		{
		case TYPE_NRM:
			NRM_C++;
			break;
		case TYPE_VGAN:
			VGN_C++;
			break;
		case TYPE_VIP:
			VIP_C++;
			break;
		default:
			break;
		}

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

	///////////Adding In service Orders to GUI::DrawingList//////////
	Order* pServiceOrd;
	int SRVCount = 0;
	int Servicesize = 0;
	Order** Service_Orders_Array = InService.toArray(Servicesize);
	SRVCount += Servicesize;
	for (int i = 0; i < Servicesize; i++)
	{
		pServiceOrd = Service_Orders_Array[i];
		pGUI->AddToDrawingList(pServiceOrd);
	}

	///////////Adding the finished Orders to GUI::DrawingList//////////
	Order* pFinishedOrd;
	NRM_FinishedCount = 0;
	VGN_FinishedCount = 0;
	VIP_FinishedCount = 0;
	int finishedsize = 0;
	Order** Finished_Orders_Array = Finished.toArray(finishedsize);
	for (int i = 0; i < finishedsize; i++)
	{
		pFinishedOrd = Finished_Orders_Array[i];
		switch (pFinishedOrd->GetType())
		{
		case TYPE_NRM:
			NRM_FinishedCount++;
			break;
		case TYPE_VGAN:
			VGN_FinishedCount++;
			break;
		case TYPE_VIP:
			VIP_FinishedCount++;
			break;
		default:
			break;
		}
		pGUI->AddToDrawingList(pFinishedOrd);
	}
}

void Restaurant::ReadInputFile(ifstream& InputFile)
{
	do
	{
		pGUI->PrintMessage("Please enter the input text file name : ", 1, 0);
		string FileName = pGUI->GetString();
		if (!(FileName.find(".txt") != string::npos))
			FileName += ".txt";
		InputFile.open(FileName, ios::in);
		if (InputFile.is_open())
		{
			int N = 0, G = 0, V = 0;
			int MaxSN = 0, MaxSG = 0, MaxSV = 0;
			int MinSN = 0, MinSG = 0, MinSV = 0;
			int MinBN = 0, MaxBN = 0, MinBG = 0, MaxBG = 0, MinBV = 0, MaxBV = 0;
			int BO = 0;
			int SN = 0, SG = 0, SV = 0;
			int autop = 0, vip_wt = 0, M = 0;
			float InjP = 0;
			int RP = 0;
			InputFile >> N >> G >> V;
			InputFile >> MinSN >> MaxSN >> MinSG >> MaxSG >> MinSV >> MaxSV;
			InputFile >> BO >> MinBN >> MaxBN >> MinBG >> MaxBG >> MinBV >> MaxBV;
			InputFile >> InjP >> RP;
			InputFile >> autop >> vip_wt;
			InputFile >> M;
			int EventCnt = M;
			Order* pOrd;
			pOrd->SetVIP_WAITANDNRM_WAIT(vip_wt, autop);
			Event* pEv;
			srand(time(NULL));

			C_count = N + G + V;
			Cook* pCook;
			int S;//the speed of the cook
			int B;//the break duration 

			//Adding the normal cooks the cooks array
			for (int i = 0; i < N; i++)
			{
				pCook = new Cook;
				pCook->SetID(i + 1);
				S = MinSN + rand() % (MaxSN - MinSN + 1);
				pCook->SetSpeed(S);
				pCook->SetOriginalSpeed(S);
				pCook->SetType(TYPE_NRM);
				B = MinBN + rand() % (MaxBN - MinBN + 1);
				pCook->SetBreakDuration(B);
				AVAILABLECOOKS.InsertEnd(pCook);
				cout << "\nCook " << pCook->GetID() << " : Speed = " << pCook->GetSpeed() << " , Break Duration = " << pCook->GetBreakDuration() << endl;
			}
			pCook->SetOrdToBreakANDRest(BO, RP);
			pCook->SetInjProp(InjP);

			// Adding the vegan cooks the cooks array
			for (int i = N; i < C_count - V; i++)
			{
				pCook = new Cook;
				pCook->SetID(i + 1);
				S = MinSG + rand() % (MaxSG - MinSG + 1);
				pCook->SetSpeed(S);
				pCook->SetOriginalSpeed(S);
				pCook->SetType(TYPE_VGAN);
				B = MinBG + rand() % (MaxBG - MinBG + 1);
				pCook->SetBreakDuration(B);
				AVAILABLECOOKS.InsertEnd(pCook);
				cout << "\nCook " << pCook->GetID() << " : Speed = " << pCook->GetSpeed() << " , Break Duration = " << pCook->GetBreakDuration() << endl;
			}

			//Adding the VIP cooks the cooks array
			for (int i = N + G; i < C_count; i++)
			{
				pCook = new Cook;
				pCook->SetID(i + 1);
				S = MinSV + rand() % (MaxSV - MinSV + 1);
				pCook->SetSpeed(S);
				pCook->SetOriginalSpeed(S);
				pCook->SetType(TYPE_VIP);
				B = MinBV + rand() % (MaxBV - MinBV + 1);
				pCook->SetBreakDuration(B);
				AVAILABLECOOKS.InsertEnd(pCook);
				cout << "\nCook " << pCook->GetID() << " : Speed = " << pCook->GetSpeed() << " , Break Duration = " << pCook->GetBreakDuration() << endl;
			}

			int EvTime = 0;
			int O_id = 0;
			CancellationEvent* pCE;

			for (int i = 0; i < EventCnt; i++)
			{
				char EventType = ' ';
				InputFile >> EventType;

				switch (EventType)
				{
				case 'R':
					char OType;
					InputFile >> OType;
					int OrderType;
					switch (OType)
					{
					case 'N':
						TotalNRMOrders++;
						OrderType = 0;
						break;
					case 'G':
						OrderType = 1;
						break;
					case 'V':
						OrderType = 2;
						break;
					default:
						break;
					}
					InputFile >> EvTime;
					InputFile >> O_id;
					int OrderSize;
					double OrderCost;
					InputFile >> OrderSize >> OrderCost;
					pEv = new ArrivalEvent(EvTime, O_id, (ORD_TYPE)OrderType, OrderSize, OrderCost);
					EventsQueue.enqueue(pEv);
					break;

				case 'X':
					InputFile >> EvTime;
					InputFile >> O_id;
					pCE = new CancellationEvent(EvTime, O_id);
					EventsQueue.enqueue(pCE);
					break;

				case 'P':
					InputFile >> EvTime;
					InputFile >> O_id;
					int ExtMoney;
					InputFile >> ExtMoney;
					pEv = new PromotionEvent(EvTime, O_id, ExtMoney);
					EventsQueue.enqueue(pEv);
					break;

				default:
					break;
				}
			}
		}
		else
		{
			pGUI->PrintMessage("Couldn't find the file. Click to enter a valid name", 1, 0);
			pGUI->waitForClick();
		}
	} while (!InputFile.is_open());

}

void Restaurant::WritingOutputFile()
{
	ofstream outputFile;
	pGUI->PrintMessage("Please enter the output text file name without .txt: ", 1, 0);
	string FileName = pGUI->GetString();
	FileName += ".txt";
	outputFile.open(FileName, ios::out);

	outputFile << "FT  ID  AT  WT  ST\n";

	int finishedOrders_Size = 0;
	float TotalWaitingTime = 0, TotalServingTime = 0;
	Order** finishedOrders = Finished.toArray(finishedOrders_Size);
	for (int i = 0; i < finishedOrders_Size; i++)
	{
		TotalWaitingTime += finishedOrders[i]->GetInServiceTime() - finishedOrders[i]->GetArrivalTime();
		TotalServingTime += finishedOrders[i]->GetFinishTime() - finishedOrders[i]->GetInServiceTime();
		outputFile << finishedOrders[i]->GetFinishTime() << "  " << finishedOrders[i]->GetID() << "   " << finishedOrders[i]->GetArrivalTime()
			<< "   " << finishedOrders[i]->GetInServiceTime() - finishedOrders[i]->GetArrivalTime() << "   "
			<< finishedOrders[i]->GetFinishTime() - finishedOrders[i]->GetInServiceTime() << endl;
	}

	outputFile << "Orders: " << NRM_FinishedCount + VGN_FinishedCount + VIP_FinishedCount << " [Norm: "
		<< NRM_FinishedCount << ", Veg: " << VGN_FinishedCount << ", VIP: " << VIP_FinishedCount << ", injured: " << injured << "]\n";
	outputFile << "Cooks: " << NRM_C + VGN_C + VIP_C << " [Norm: " << NRM_C << ", Veg: " << VGN_C << ", VIP: " << VIP_C << "]\n";
	outputFile << "Avg Wait = " << TotalWaitingTime / finishedOrders_Size << ",  Avg Serv = " << TotalServingTime / finishedOrders_Size << endl;
	outputFile << "Urgent orders: " << UrgentOrders << ",  Auto-promoted: " << (float(AutoPromoted) / float(TotalNRMOrders)) * 100.0 << "%\n";

	pGUI->PrintMessage("Output text file is generated, click to END the program.", 1, 0);
	pGUI->waitForClick();
}

void  Restaurant::AddtoVIPQueue(Order* po)	//adds an order to the vip orders queue
{	// To calculate the Priority of the order
	int p = po->GetOrderMoney() + po->GetOrderSize() + 2 * po->GetArrivalTime();
	WaitingVIP.enqueue(po, p);
}

void  Restaurant::AddtoNormalQueue(Order* po) //adds an order to the normal orders queue
{
	WaitingNormal.enqueue(po);
}

void  Restaurant::AddtoVeganQueue(Order* po) //adds an order to the vegan orders queue
{
	WaitingVegan.enqueue(po);
}

bool  Restaurant::DeleteNormalQueue(int id) // Removes order from normal queue if cancelled
{
	Order* CancellatedOrder;
	bool IsFound = WaitingNormal.SearchForOrder(id, CancellatedOrder);
	return IsFound;
}

Order*& Restaurant::PromotOrder(int id, double Extra)
{
	Order* PromotedOrder = NULL;
	bool IsFound = WaitingNormal.SearchForOrder(id, PromotedOrder);
	if (IsFound)
	{
		PromotedOrder->SetOrderMoney(PromotedOrder->GetOrderMoney() + Extra);
		PromotedOrder->SetType(TYPE_VIP);
		cout << "\nOrder " << PromotedOrder->GetID() << " has been promoted to VIP with extra money = " << Extra << endl;
	}
	return PromotedOrder;
}

void Restaurant::UpdateCooks()
{
	int demo = 0;
	int C_size = 0;
	Cook** CookArr = AVAILABLECOOKS.toArray(C_size);
	////////////////////////////////UBDATE SERVING COOKS////////////////////////////////
	for (int i = 0; i < C_size; i++)
	{
		if (CookArr[i]->GetAssignedOrder() && !CookArr[i]->GetInBreak())
		{
			if (!CookArr[i]->GetInjured())
			{
				if ((CookArr[i]->GetOrdToBreakANDRest(demo) > CookArr[i]->GetCurrOrd()) && (CookArr[i]->GetAssignedOrder()->GetStatus() == DONE))
				{	//Check for cooks should be not assigned
					CookArr[i]->SetCurrOrd(CookArr[i]->GetCurrOrd() + 1);
					CookArr[i]->SetAssignedOrder(NULL);
					AVAILABLECOOKS.DeleteNode(CookArr[i]);
					AVAILABLECOOKS.InsertEnd(CookArr[i]);  //resorting available cooks list
				}
				if (CookArr[i]->GetOrdToBreakANDRest(demo) == CookArr[i]->GetCurrOrd())
				{   //Check for cooks that should be in break
					CookArr[i]->SetInBreak(true);
					CookArr[i]->SetCurrOrd(0);
					CookArr[i]->SetBreakTS(CurrentTimeStep);
					CookArr[i]->ResetSpeed();
					if (CookArr[i]->GetType() == TYPE_NRM) NRM_C--;
					else if (CookArr[i]->GetType() == TYPE_VGAN) VGN_C--;
					else VIP_C--;
					AVAILABLECOOKS.DeleteNode(CookArr[i]);
					InBreakCooks.InsertBeg(CookArr[i]); // Removing in break cooks from available list& adding them to in break list
				}
			}
			else if (CookArr[i]->GetAssignedOrder()->GetStatus() == DONE)
			{
				CookArr[i]->SetCurrOrd(CookArr[i]->GetCurrOrd() + 1);
				CookArr[i]->SetAssignedOrder(NULL);
				CookArr[i]->SetCurrOrd(0);
				AVAILABLECOOKS.DeleteNode(CookArr[i]);
				InjuredCooks.InsertEnd(CookArr[i]);
				CookArr[i]->SetInjured(false);
				if (CookArr[i]->GetType() == TYPE_NRM) NRM_C--;
				else if (CookArr[i]->GetType() == TYPE_VGAN) VGN_C--;
				else VIP_C--;
			}
		}
	}

	////////////////////////////////UBDATE INBREAK COOKS////////////////////////////////
	int breaksize;
	Cook** CookinBreak = InBreakCooks.toArray(breaksize);
	for (int i = 0; i < breaksize; i++)//Check for cooks should go back to available list
		if (CurrentTimeStep - CookinBreak[i]->GetBreakTS() == CookinBreak[i]->GetBreakDuration())
		{
			InBreakCooks.DeleteNode(CookinBreak[i]);
			CookinBreak[i]->SetInBreak(false);
			AVAILABLECOOKS.InsertBeg(CookinBreak[i]);
			if (CookinBreak[i]->GetType() == TYPE_NRM) NRM_C++;
			else if (CookinBreak[i]->GetType() == TYPE_VGAN) VGN_C++;
			else VIP_C++;
			cout << "\nCook " << CookinBreak[i]->GetID() << " finished his break at " << CurrentTimeStep << endl;
		}

	///////////////////////////////To Make Cook injured///////////////////////////////
	srand(time(NULL));
	float R = (rand() % 101) / 100.0;
	float inj = CookArr[0]->GetInjProp();
	int rp;
	CookArr[0]->GetOrdToBreakANDRest(rp);
	int newFinishedTime = 0;
	//cout << "\nR = " << R << endl;
	if (R <= inj)
	{
		for (int i = 0; i < C_size; i++)  //searching for the first busy Cook To Make him injured
		{
			if (CookArr[i]->GetAssignedOrder() && !CookArr[i]->GetInjured())
			{
				if (CookArr[i]->GetSpeed() > 1)
				{
					newFinishedTime = CurrentTimeStep + ((CookArr[i]->GetAssignedOrder()->GetOrderSize() - ((CurrentTimeStep - CookArr[i]->GetAssignedOrder()->GetInServiceTime()) * CookArr[i]->GetSpeed())) / (CookArr[i]->GetSpeed() / 2));
					CookArr[i]->SetSpeed(CookArr[i]->GetSpeed() / 2);
				}
				else
				{
					newFinishedTime = CurrentTimeStep + ((CookArr[i]->GetAssignedOrder()->GetOrderSize() - ((CurrentTimeStep - CookArr[i]->GetAssignedOrder()->GetInServiceTime()) * CookArr[i]->GetSpeed())));
					CookArr[i]->SetSpeed(1);
				}
				CookArr[i]->GetAssignedOrder()->SetFinishTime(newFinishedTime);
				CookArr[i]->SetToBackFromRest(newFinishedTime + rp);
				CookArr[i]->SetInjured(true);
				cout << "\nCook " << CookArr[i]->GetID() << " got injured at TS " << CurrentTimeStep << " and will finish his order and go te rest at TS " << newFinishedTime << endl;
				break;
			}
			////else
			////{
			//	AVAILABLECOOKS.DeleteNode(CookArr[i]);
			//	CookArr[i]->SetSpeed(CookArr[i]->GetSpeed() / 2);
			//	CookArr[i]->SetToBackFromRest(rp);
			//	InjuredCooks.InsertEnd(CookArr[i]);
			//	if (CookArr[i]->GetType() == TYPE_NRM) NRM_C--;
			//	else if (CookArr[i]->GetType() == TYPE_VGAN) VGN_C--;
			//	else VIP_C--;
			////}
		}
	}
	/////////////////////////////UBDATE INJURED COOKS//////////////////////////////////
	Cook** Cookinjured = InjuredCooks.toArray(C_size);
	for (int i = 0; i < C_size; i++)//Check for cooks should go back to available list
		if (CurrentTimeStep == Cookinjured[i]->GetToBackFromRest())
		{
			InjuredCooks.DeleteNode(Cookinjured[i]);
			Cookinjured[i]->ResetSpeed();
			AVAILABLECOOKS.InsertSorted(Cookinjured[i]);
			if (Cookinjured[i]->GetType() == TYPE_NRM) NRM_C++;
			else if (Cookinjured[i]->GetType() == TYPE_VGAN) VGN_C++;
			else VIP_C++;
			cout << "\nCook " << Cookinjured[i]->GetID() << " finished his rest at TS " << CurrentTimeStep << endl;
		}
}

void Restaurant::UpdateInServiceOrders()
{
	int O_size = 0;
	Order** pOrd = InService.toArray(O_size);
	for (int i = 0; i < O_size; i++)
	{
		if (CurrentTimeStep == pOrd[i]->GetFinishTime())
		{
			pOrd[i]->SetStatus(DONE);
			InService.DeleteNode(pOrd[i]);
			Finished.enqueue(pOrd[i]);
			if (pOrd[i]->GetType() == TYPE_NRM) NRM_FinishedCount++;
			else if (pOrd[i]->GetType() == TYPE_VGAN) VGN_FinishedCount++;
			else VIP_FinishedCount++;
			cout << "\nOrder " << pOrd[i]->GetID() << " is finished at TS " << CurrentTimeStep << endl;
		}
	}
}

void Restaurant::Assigning()
{
	int C_size = 0;
	Cook** CookArr = AVAILABLECOOKS.toArray(C_size);
	Cook* pCook;
	Order* pOrd;
	///////////////////////////////Seperating free cooks of each type///////////////////////////////
	List<Cook*> FreeNormal;
	List<Cook*> FreeVegan;
	List<Cook*> FreeVIP;
	List<Cook*> FreeAll;
	bool nrm = true, vip = true, vgn = true;
	if (!(WaitingVIP.peekFront(pOrd) && pOrd->GetArrivalTime() <= CurrentTimeStep))
		if (!(WaitingVegan.peekFront(pOrd) && pOrd->GetArrivalTime() <= CurrentTimeStep))
			if (!(WaitingNormal.peekFront(pOrd) && pOrd->GetArrivalTime() <= CurrentTimeStep))
				return;
	for (int i = 0; i < C_size; i++)
	{
		if (!CookArr[i]->GetAssignedOrder())
		{
			AVAILABLECOOKS.DeleteNode(CookArr[i]);
			if (CookArr[i]->GetType() == TYPE_VIP)  FreeVIP.InsertEnd(CookArr[i]);
			else if (CookArr[i]->GetType() == TYPE_VGAN) FreeVegan.InsertEnd(CookArr[i]);
			else FreeNormal.InsertEnd(CookArr[i]);
			FreeAll.InsertEnd(CookArr[i]);
		}
	}

	///////////////////////////////////Assigning VIP orders///////////////////////////////////
	while (WaitingVIP.peekFront(pOrd) && pOrd->GetArrivalTime() <= CurrentTimeStep)//while there is VIP orders in waiting list till this current time step
	{
		pCook = !FreeVIP.IsEmpty() ? FreeVIP.RemoveFirst() : //Looks first for a VIP cook then Vegan then Normal
			(!FreeVegan.IsEmpty() ? FreeVegan.RemoveFirst() :
			(!FreeNormal.IsEmpty() ? FreeNormal.RemoveFirst() : NULL));
		if (pCook)
		{
			WaitingVIP.dequeue(pOrd);
			pCook->SetAssignedOrder(pOrd);
			pOrd->SetInServiceTime(CurrentTimeStep);
			pOrd->SetFinishTime(CurrentTimeStep + ceil(float(pOrd->GetOrderSize()) / pCook->GetSpeed()));
			pOrd->SetStatus(SRV);
			InService.InsertEnd(pOrd);
			AVAILABLECOOKS.InsertSorted(pCook);
			FreeAll.DeleteNode(pCook);
			cout << "\nOrder " << pOrd->GetID() << " of size " << pOrd->GetOrderSize() << " is assigned to cook " << pCook->GetID() << " with speed " << pCook->GetSpeed() << " and should be finished at TS " << pOrd->GetFinishTime() << endl;
			continue;
		}
		else if (pOrd->GetUrgent())//if there are no cooks except for injured and in break
		{
			if (InBreakCooks.GetCount())
			{//if there are cooks in break
				pCook = InBreakCooks.Remove();
				pCook->SetInBreak(false);
			}
			else if (InjuredCooks.GetCount())
			{//if there are cooks in rest
				pCook = InjuredCooks.Remove();
				pCook->SetInjured(false);
			}

			if (pCook)
			{
				WaitingVIP.dequeue(pOrd);
				pCook->SetAssignedOrder(pOrd);
				pOrd->SetInServiceTime(CurrentTimeStep);
				pOrd->SetFinishTime(CurrentTimeStep + ceil(float(pOrd->GetOrderSize()) / pCook->GetSpeed()));
				pOrd->SetStatus(SRV);
				InService.InsertEnd(pOrd);
				AVAILABLECOOKS.InsertSorted(pCook);
				if (pCook->GetType() == TYPE_NRM) NRM_C++;
				else if (pCook->GetType() == TYPE_VGAN) VGN_C++;
				else VIP_C++;
				cout << "\nOrder " << pOrd->GetID() << " of size " << pOrd->GetOrderSize() << " is assigned to cook " << pCook->GetID() << " with speed " << pCook->GetSpeed() << " and should be finished at TS " << pOrd->GetFinishTime() << endl;
			}
			else return;
		}
		else return;
	}
	///////////////////////////////////Assigning Vegan Orders///////////////////////////////////
	while (WaitingVegan.peekFront(pOrd) && pOrd->GetArrivalTime() <= CurrentTimeStep)//while there is vegan orders in waiting list till this current time step
	{
		pCook = !FreeVegan.IsEmpty() ? FreeVegan.RemoveFirst() : NULL; //Looks first for a Vegan cook
		if (pCook)
		{
			WaitingVegan.dequeue(pOrd);
			pCook->SetAssignedOrder(pOrd);
			pOrd->SetInServiceTime(CurrentTimeStep);
			pOrd->SetFinishTime(CurrentTimeStep + ceil(float(pOrd->GetOrderSize()) / pCook->GetSpeed()));
			pOrd->SetStatus(SRV);
			InService.InsertEnd(pOrd);
			AVAILABLECOOKS.InsertSorted(pCook);
			FreeAll.DeleteNode(pCook);
			cout << "\nOrder " << pOrd->GetID() << " of size " << pOrd->GetOrderSize() << " is assigned to cook " << pCook->GetID() << " with speed " << pCook->GetSpeed() << " and should be finished at TS " << pOrd->GetFinishTime() << endl;
			continue;
		}
		else break;
	}
	///////////////////////////////////Assigning Normal Orders///////////////////////////////////
	while (WaitingNormal.peekFront(pOrd) && pOrd->GetArrivalTime() <= CurrentTimeStep)//while there is more normal orders in waiting list till this current time step
	{
		pCook = !FreeNormal.IsEmpty() ? FreeNormal.RemoveFirst() : //Looks first for a Normal cook then VIP 
			(!FreeVIP.IsEmpty() ? FreeVIP.RemoveFirst() : NULL);
		if (pCook)
		{
			WaitingNormal.dequeue(pOrd);
			pCook->SetAssignedOrder(pOrd);
			pOrd->SetInServiceTime(CurrentTimeStep);
			pOrd->SetFinishTime(CurrentTimeStep + ceil(float(pOrd->GetOrderSize()) / pCook->GetSpeed()));
			pOrd->SetStatus(SRV);
			InService.InsertEnd(pOrd);
			AVAILABLECOOKS.InsertSorted(pCook);
			FreeAll.DeleteNode(pCook);
			cout << "\nOrder " << pOrd->GetID() << " of size " << pOrd->GetOrderSize() << " is assigned to cook " << pCook->GetID() << " with speed " << pCook->GetSpeed() << " and should be finished at TS " << pOrd->GetFinishTime() << endl;
			continue;
		}
		else break;
	}
	//while (!FreeVIP.IsEmpty()) AVAILABLECOOKS.InsertSorted(FreeVIP.RemoveFirst());
	//while (!FreeVegan.IsEmpty()) AVAILABLECOOKS.InsertSorted(FreeVegan.RemoveFirst());
	//while (!FreeNormal.IsEmpty()) AVAILABLECOOKS.InsertSorted(FreeNormal.RemoveFirst());

	while (!FreeAll.IsEmpty()) AVAILABLECOOKS.InsertSorted(FreeAll.Remove());
}

void Restaurant::UpdateWaiting() //auto promote waiting nomral to vip & waiting vip to urgent
{
	int VIP_size = 0;
	Order** VIP_Order = WaitingVIP.toArray(VIP_size);
	Order* pOrd;
	WaitingNormal.peekFront(pOrd);
	int vip_wt, autop;
	vip_wt = pOrd->GetVIP_WAITANDNRM_WAIT(autop);
	///////////////////////////////Update Urgent vip orders of this time step//////////////////
	for (int i = 0; i < VIP_size; i++)
	{
		if (CurrentTimeStep - VIP_Order[i]->GetArrivalTime() > vip_wt && !VIP_Order[i]->GetUrgent())
		{
			pOrd = VIP_Order[i];
			WaitingVIP.DeleteNode(VIP_Order[i]->GetID());
			pOrd->SetUrgent(true);
			WaitingVIP.enqueue(pOrd, pOrd->GetArrivalTime());	//make urgent orders at the first of the queue
			UrgentOrders++;
			cout << "\nOrder " << pOrd->GetID() << " turned urgent with total " << UrgentOrders << " urgent orders\n";
		}
	}
	////////////////////////////////////Automatically promoted for Normal/////////////////////////////////////
	while (WaitingNormal.peekFront(pOrd) && (CurrentTimeStep - (pOrd->GetArrivalTime()) > autop))
	{
		WaitingNormal.dequeue(pOrd);
		pOrd->SetType(TYPE_VIP);
		WaitingVIP.enqueue(pOrd, pOrd->GetArrivalTime() + pOrd->GetOrderMoney() + pOrd->GetOrderSize());
		AutoPromoted++;
		cout << "\nOrder " << pOrd->GetID() << " is autopromoted to VIP with total " << AutoPromoted << " autopromoted orders\n";
	}
}

void Restaurant::PrintData()
{
	string waitingNRM = to_string(NRM_OrdCount);
	string waitingVGN = to_string(VGN_OrdCount);
	string waitingVIP = to_string(VIP_OrdCount);

	string finishedNRM = to_string(NRM_FinishedCount);
	string finishedVGN = to_string(VGN_FinishedCount);
	string finishedVIP = to_string(VIP_FinishedCount);

	string C_NRM = to_string(NRM_C);
	string C_VGN = to_string(VGN_C);
	string C_VIP = to_string(VIP_C);

	string timestep = to_string(CurrentTimeStep);

	pGUI->PrintMessage("TS: " + timestep, 1, 0);	//print current timestep
	pGUI->PrintMessage("Waiting: (Normal = " + waitingNRM + "), (Vegan = " + waitingVGN + "), (VIP = " + waitingVIP + ")", 2, 0);	//print waiting orders numbers
	pGUI->PrintMessage("Cooks: (Normal = " + C_NRM + "), (Vegan = " + C_VGN + "), (VIP = " + C_VIP + ")", 3, 0);		//Prints the number if cooks
	pGUI->PrintMessage("Finished: (Normal = " + finishedNRM + "), (Vegan = " + finishedVGN + "), (VIP = " + finishedVIP + ")", 5, 0);	//print finished orders numbers

	int csize = 0, i = 0;
	Cook** C_Arr = AVAILABLECOOKS.toArray(csize);

	for (int j = 0, count = 0; j < csize; j++)
	{
		if (C_Arr[j]->GetAssignedOrder())
		{
			char ctype;
			if (C_Arr[j]->GetType() == 0) ctype = 'N';
			else if (C_Arr[j]->GetType() == 1) ctype = 'G';
			else ctype = 'V';

			char otype;
			if (C_Arr[j]->GetAssignedOrder()->GetType() == 0) otype = 'N';
			else if (C_Arr[j]->GetAssignedOrder()->GetType() == 1) otype = 'G';
			else otype = 'V';

			string C_ID = to_string(C_Arr[j]->GetID());
			string O_ID = to_string(C_Arr[j]->GetAssignedOrder()->GetID());

			pGUI->PrintMessage(ctype + C_ID + "(" + otype + O_ID + ")", 4, count);
			count++;
		}
	}
}