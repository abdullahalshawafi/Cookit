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
	OriginalNRMOrders = 0;
	NRM_FinishedCount = 0;
	VGN_FinishedCount = 0;
	VIP_FinishedCount = 0;
	NRM_C = 0;
	VIP_C = 0;
	VGN_C = 0;
	Injured_C = 0;
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

	while (notFinished)	//as long as events queue, injured, and in break cooks lists are not empty yet
	{
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step
		UpdateInService();
		UpdateCooks();
		Assigning();
		UpdateWaiting();
		FillDrawingList();
		if (mode != MODE_SLNT)
		{
			pGUI->UpdateInterface();
			PrintData();
		}
		if (mode == MODE_INTR)
			pGUI->waitForClick();
		else if (mode == MODE_STEP)
			Sleep(1000);
		pGUI->ResetDrawingList();
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
	//Let's add ALL Cooks to GUI::DrawingList
	int C_size = 0;
	VIP_C = 0;
	VGN_C = 0;
	NRM_C = 0;
	Cook** CookArr = WorkingCooks.toArray(C_size);
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
	int Servicesize = 0;
	Order** Service_Orders_Array = InService.toArray(Servicesize);
	for (int i = 0; i < Servicesize; i++)
	{
		pServiceOrd = Service_Orders_Array[i];
		pGUI->AddToDrawingList(pServiceOrd);
	}

	///////////Adding the finished Orders to GUI::DrawingList//////////
	Order* pFinishedOrd;
	int finishedsize = 0;
	Order** Finished_Orders_Array = Finished.toArray(finishedsize);
	for (int i = 0; i < finishedsize; i++)
	{
		pFinishedOrd = Finished_Orders_Array[i];
		pGUI->AddToDrawingList(pFinishedOrd);
	}
}

//////////////////////////////////  I/O files handling functions   /////////////////////////////

void Restaurant::ReadInputFile(ifstream& InputFile)
{
	do
	{
		pGUI->PrintMessage("Please enter the input text file name : ", 1, 0);
		string FileName = "Input_Tests/";
		FileName += pGUI->GetString();
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
			int autop = 0, vip_wt = 0, EventCnt = 0;
			float InjP = 0;
			int RP = 0;
			InputFile >> N >> G >> V;
			InputFile >> MinSN >> MaxSN >> MinSG >> MaxSG >> MinSV >> MaxSV;
			InputFile >> BO >> MinBN >> MaxBN >> MinBG >> MaxBG >> MinBV >> MaxBV;
			InputFile >> InjP >> RP;
			InputFile >> autop >> vip_wt;
			InputFile >> EventCnt;

			Order* pOrd;
			pOrd->SetVIP_WAITANDNRM_WAIT(vip_wt, autop);
			Event* pEv;
			srand(time(NULL));

			C_count = N + G + V;
			Cook* pCook;
			int S;	//the speed of the cook
			int B;	//the break duration 

			//Adding the normal cooks the working cooks list
			for (int i = 0; i < N; i++)
			{
				S = MinSN + rand() % (MaxSN - MinSN + 1);
				B = MinBN + rand() % (MaxBN - MinBN + 1);
				pCook = new Cook;
				pCook->SetID(i + 1);
				pCook->SetSpeed(S);
				pCook->SetOriginalSpeed(S);
				pCook->SetType(TYPE_NRM);
				pCook->SetBreakDuration(B);
				WorkingCooks.InsertEnd(pCook);
				cout << "\nCook " << pCook->GetID() << " : Speed = " << pCook->GetSpeed() << " , Break Duration = " << pCook->GetBreakDuration() << endl;
			}

			// Adding the vegan cooks the working cooks list
			for (int i = N; i < C_count - V; i++)
			{
				S = MinSG + rand() % (MaxSG - MinSG + 1);
				B = MinBG + rand() % (MaxBG - MinBG + 1);
				pCook = new Cook;
				pCook->SetID(i + 1);
				pCook->SetSpeed(S);
				pCook->SetOriginalSpeed(S);
				pCook->SetType(TYPE_VGAN);
				pCook->SetBreakDuration(B);
				WorkingCooks.InsertEnd(pCook);
				cout << "\nCook " << pCook->GetID() << " : Speed = " << pCook->GetSpeed() << " , Break Duration = " << pCook->GetBreakDuration() << endl;
			}

			//Adding the VIP cooks the working cooks list
			for (int i = N + G; i < C_count; i++)
			{
				S = MinSG + rand() % (MaxSG - MinSG + 1);
				B = MinBG + rand() % (MaxBG - MinBG + 1);
				pCook = new Cook;
				pCook->SetID(i + 1);
				pCook->SetSpeed(S);
				pCook->SetOriginalSpeed(S);
				pCook->SetType(TYPE_VIP);
				pCook->SetBreakDuration(B);
				WorkingCooks.InsertEnd(pCook);
				cout << "\nCook " << pCook->GetID() << " : Speed = " << pCook->GetSpeed() << " , Break Duration = " << pCook->GetBreakDuration() << endl;
			}

			pCook->SetOrdToBreakANDRest(BO, RP);
			pCook->SetInjProp(InjP);

			int EvTime = 0;
			int O_id = 0;

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
						OriginalNRMOrders++;
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
					pEv = new CancellationEvent(EvTime, O_id);
					EventsQueue.enqueue(pEv);
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
	pGUI->PrintMessage("Please enter the output text file name: ", 1, 0);
	string FileName = "Output_Tests/";
	FileName += pGUI->GetString();
	if (!(FileName.find(".txt") != string::npos))
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
		<< NRM_FinishedCount << ", Veg: " << VGN_FinishedCount << ", VIP: " << VIP_FinishedCount << "]\n";
	outputFile << "Cooks: " << NRM_C + VGN_C + VIP_C << " [Norm: " << NRM_C << ", Veg: " << VGN_C << ", VIP: " << VIP_C << ", injured: " << Injured_C << "]\n";
	outputFile << "Avg Wait = " << TotalWaitingTime / finishedOrders_Size << ",  Avg Serv = " << TotalServingTime / finishedOrders_Size << endl;
	outputFile << "Urgent orders: " << UrgentOrders << ",  Auto-promoted: " << (float(AutoPromoted) / float(OriginalNRMOrders)) * 100.0 << "%\n";

	pGUI->PrintMessage("Output text file is generated, click to END the program.", 1, 0);
	pGUI->waitForClick();
}

//////////////////////////////////  Orders lists handling functions   /////////////////////////////

void  Restaurant::AddtoVIPQueue(Order* po)	//adds an order to the vip orders queue
{	//To calculate the Priority of the order we used the following priority equation
	float p = po->GetOrderMoney() / float(po->GetOrderSize() * po->GetArrivalTime());
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

bool  Restaurant::DeletefromNormalQueue(int id) // Removes order from normal queue if cancelled
{
	//First we search for the order and remove it from the queue
	Order* CancellatedOrder = NULL;
	int WaitingNormal_Count = 0;
	WaitingNormal.toArray(WaitingNormal_Count);
	for (int i = 0; i < WaitingNormal_Count; i++)
	{
		Order* temp;
		WaitingNormal.dequeue(temp);
		if (temp->GetID() == id)
		{
			CancellatedOrder = temp;
			cout << "\nOrder " << CancellatedOrder->GetID() << " has been cancelled at TS " << CurrentTimeStep << endl;
			continue;
		}
		WaitingNormal.enqueue(temp);
	}
	return (CancellatedOrder != NULL);
}

Order*& Restaurant::PromotOrder(int id, double Extra)
{
	//First we search for the order and remove it from the queue
	Order* PromotedOrder = NULL;
	int WaitingNormal_Count = 0;
	WaitingNormal.toArray(WaitingNormal_Count);
	for (int i = 0; i < WaitingNormal_Count; i++)
	{
		Order* temp;
		WaitingNormal.dequeue(temp);
		if (temp->GetID() == id)
		{
			PromotedOrder = temp;
			continue;
		}
		WaitingNormal.enqueue(temp);
	}

	//Secondly set the new data members
	if (PromotedOrder)
	{
		PromotedOrder->SetOrderMoney(PromotedOrder->GetOrderMoney() + Extra);
		PromotedOrder->SetType(TYPE_VIP);
		PromotedOrder->SetPromotionTS(CurrentTimeStep);
		cout << "\nOrder " << PromotedOrder->GetID() << " has been promoted to VIP at TS " << CurrentTimeStep << " with extra money = " << Extra << endl;
	}
	return PromotedOrder;
}

//////////////////////////////////  Simulation functions   /////////////////////////////

void Restaurant::UpdateInService()
{
	int O_size = 0, FinishedOrdersCount = 0;	//O_size is for the no. of in-service orders, and FinishedOrdersCount is for the no. of finished orders in this TS
	Order** pOrd = InService.toArray(O_size);

	for (int i = 0; i < O_size; i++)
		if (CurrentTimeStep == pOrd[i]->GetFinishTime())
			FinishedOrdersCount++;

	Order** finishedOrders = nullptr;	//finishedOrders is an array of pointer to finished orders pointers
	if (FinishedOrdersCount)
		finishedOrders = new Order * [FinishedOrdersCount];		//Allocating the array of pointers in case there are finished orders in this TS

	for (int i = 0, j = 0; i < O_size; i++)
	{
		if (CurrentTimeStep == pOrd[i]->GetFinishTime())
		{	//Check if the order should be finished at this TS
			pOrd[i]->SetStatus(DONE);
			InService.DeleteNode(pOrd[i]);
			finishedOrders[j] = pOrd[i];	//Adding finished orders to the temporary array
			j++;
			if (pOrd[i]->GetType() == TYPE_NRM) NRM_FinishedCount++;
			else if (pOrd[i]->GetType() == TYPE_VGAN) VGN_FinishedCount++;
			else VIP_FinishedCount++;
			cout << "\nOrder " << pOrd[i]->GetID() << " is finished at TS " << CurrentTimeStep << endl;
		}
	}

	//Enqueue the finished orders to the Finished Queue in an invered order. This is done to sort the Finished Queue in the right order for the output file
	for (int i = FinishedOrdersCount - 1; i >= 0; i--)
		Finished.enqueue(finishedOrders[i]);

	if (FinishedOrdersCount)
		delete[] finishedOrders;
}

void Restaurant::UpdateCooks()
{
	int demo = 0;
	int C_size = 0;
	Cook** CookArr = WorkingCooks.toArray(C_size);
	////////////////////////////////UBDATE SERVING COOKS////////////////////////////////
	for (int i = 0; i < C_size; i++)
	{
		if (CookArr[i]->GetAssignedOrder() && !CookArr[i]->GetInBreak())
		{
			if (!CookArr[i]->GetInjured())
			{
				if ((CookArr[i]->GetOrdToBreakANDRest(demo) > CookArr[i]->GetFinishedOrders()) && (CookArr[i]->GetAssignedOrder()->GetStatus() == DONE))
				{	//Check for cooks that finished their assigned order and ready to get assigned again
					CookArr[i]->SetAssignedOrder(NULL);
					WorkingCooks.DeleteNode(CookArr[i]);
					WorkingCooks.InsertSorted(CookArr[i]);  //resorting working cooks list
				}
				else if (CookArr[i]->GetOrdToBreakANDRest(demo) == CookArr[i]->GetFinishedOrders() && (CookArr[i]->GetAssignedOrder()->GetStatus() == DONE))
				{   //Check for injured cooks that finished their assigned order and needs to go to break
					CookArr[i]->SetAssignedOrder(NULL);
					CookArr[i]->SetInBreak(true);
					CookArr[i]->SetFinishedOrders(0);
					CookArr[i]->SetBreakTS(CurrentTimeStep);
					CookArr[i]->ResetSpeed();
					WorkingCooks.DeleteNode(CookArr[i]);
					InBreakCooks.InsertEnd(CookArr[i]); //Removing cooks from working list & adding them to in break list
				}
			}
			else if (CookArr[i]->GetAssignedOrder()->GetStatus() == DONE)
			{	//Check for injured cooks that finished their assigned order and needs to go to rest
				CookArr[i]->SetAssignedOrder(NULL);
				CookArr[i]->SetFinishedOrders(0);
				WorkingCooks.DeleteNode(CookArr[i]);
				InjuredCooks.InsertEnd(CookArr[i]);		//Removing cooks from working list & adding them to injured list
				CookArr[i]->SetInjured(false);
			}
		}
	}

	////////////////////////////////UBDATE INBREAK COOKS////////////////////////////////
	int breaksize;
	Cook** CooksinBreak = InBreakCooks.toArray(breaksize);
	for (int i = 0; i < breaksize; i++)
		if (CurrentTimeStep - CooksinBreak[i]->GetBreakTS() == CooksinBreak[i]->GetBreakDuration())
		{	//Check for cooks that should go back to working list
			CooksinBreak[i]->SetInBreak(false);
			InBreakCooks.DeleteNode(CooksinBreak[i]);
			WorkingCooks.InsertSorted(CooksinBreak[i]);	//Removing cooks from working list & adding them to in break list
			cout << "\nCook " << CooksinBreak[i]->GetID() << " finished his break at TS " << CurrentTimeStep << endl;
		}

	///////////////////////////////To Make Cook injured///////////////////////////////
	srand(time(NULL));
	float R = (rand() % 101) / 100.0;
	float inj = CookArr[0]->GetInjProp();
	int RestPeriod;
	CookArr[0]->GetOrdToBreakANDRest(RestPeriod);
	int newFinishedTime = 0;

	if (R <= inj)
	{
		for (int i = 0; i < C_size; i++)  //searching for the first busy cook to make him injured
		{
			if (CookArr[i]->GetAssignedOrder() && !CookArr[i]->GetInjured())	//we assumed that a cook can't get injured more than once
			{
				if (CookArr[i]->GetSpeed() > 1)
				{
					newFinishedTime = CurrentTimeStep + ceil(float(CookArr[i]->GetAssignedOrder()->GetOrderSize() - ((CurrentTimeStep - CookArr[i]->GetAssignedOrder()->GetInServiceTime()) * CookArr[i]->GetSpeed())) / (CookArr[i]->GetSpeed() / 2));
					CookArr[i]->SetSpeed(CookArr[i]->GetSpeed() / 2);
				}
				else
				{
					CookArr[i]->SetSpeed(1);
					newFinishedTime = CurrentTimeStep + ceil(float(CookArr[i]->GetAssignedOrder()->GetOrderSize() - ((CurrentTimeStep - CookArr[i]->GetAssignedOrder()->GetInServiceTime()) * CookArr[i]->GetSpeed())) / (CookArr[i]->GetSpeed()));
				}
				CookArr[i]->GetAssignedOrder()->SetFinishTime(newFinishedTime);
				CookArr[i]->SetBackFromRest(newFinishedTime + RestPeriod);
				CookArr[i]->SetInjured(true);
				Injured_C++;
				cout << "\nCook " << CookArr[i]->GetID() << " got injured at TS " << CurrentTimeStep << " and will finish order " << CookArr[i]->GetAssignedOrder()->GetID() << " and go te rest at TS " << newFinishedTime << endl;
				break;
			}
		}
	}

	/////////////////////////////UBDATE INJURED COOKS//////////////////////////////////
	Cook** Cookinjured = InjuredCooks.toArray(C_size);
	for (int i = 0; i < C_size; i++)
		if (CurrentTimeStep == Cookinjured[i]->GetBackFromRest())
		{	//Check for cooks that should go back to working list
			Cookinjured[i]->ResetSpeed();
			Cookinjured[i]->SetInjured(false);
			InjuredCooks.DeleteNode(Cookinjured[i]);
			WorkingCooks.InsertSorted(Cookinjured[i]);		//Removing cooks from working list & adding them to injured list
			cout << "\nCook " << Cookinjured[i]->GetID() << " finished his rest at TS " << CurrentTimeStep << endl;
		}
}

void Restaurant::Assigning()
{
	int C_size = 0;
	Cook** CookArr = WorkingCooks.toArray(C_size);
	Cook* pCook;
	Order* pOrd;
	///////////////////////////////Seperating free cooks of each type///////////////////////////////
	List<Cook*> FreeNormal;
	List<Cook*> FreeVegan;
	List<Cook*> FreeVIP;

	if (!(WaitingVIP.peekFront(pOrd) && pOrd->GetArrivalTime() <= CurrentTimeStep))					//
		if (!(WaitingVegan.peekFront(pOrd) && pOrd->GetArrivalTime() <= CurrentTimeStep))			//If there are no orders arrived at the current 
			if (!(WaitingNormal.peekFront(pOrd) && pOrd->GetArrivalTime() <= CurrentTimeStep))		//timestep, return;
				return;																				//

	for (int i = 0; i < C_size; i++)		//Filling the free cooks lists in orders
	{
		if (!CookArr[i]->GetAssignedOrder())
		{	//Check if the cook is free
			WorkingCooks.DeleteNode(CookArr[i]);
			if (CookArr[i]->GetType() == TYPE_VIP)  FreeVIP.InsertEnd(CookArr[i]);
			else if (CookArr[i]->GetType() == TYPE_VGAN) FreeVegan.InsertEnd(CookArr[i]);
			else FreeNormal.InsertEnd(CookArr[i]);
		}
	}

	///////////////////////////////////Assigning VIP orders///////////////////////////////////
	while (WaitingVIP.peekFront(pOrd) && pOrd->GetArrivalTime() <= CurrentTimeStep)		//while there is VIP orders in waiting list till this current time step
	{
		pCook = !FreeVIP.IsEmpty() ? FreeVIP.RemoveFirst() :		//Looks first for a VIP cook then Normal then Vegan
			(!FreeNormal.IsEmpty() ? FreeNormal.RemoveFirst() :
			(!FreeVegan.IsEmpty() ? FreeVegan.RemoveFirst() : NULL));

		if (pCook)
		{
			WaitingVIP.dequeue(pOrd);
			pCook->SetAssignedOrder(pOrd);
			pCook->SetFinishedOrders(pCook->GetFinishedOrders() + 1); //Incementing the number of orders that the cook has done
			pOrd->SetInServiceTime(CurrentTimeStep);
			pOrd->SetFinishTime(CurrentTimeStep + ceil(float(pOrd->GetOrderSize()) / pCook->GetSpeed()));
			pOrd->SetStatus(SRV);
			InService.InsertEnd(pOrd);
			WorkingCooks.InsertSorted(pCook);	//Returning the cook back to the working list
			cout << "\nOrder " << pOrd->GetID() << " of size " << pOrd->GetOrderSize() << " is assigned to cook " << pCook->GetID() << " with speed " << pCook->GetSpeed() << " and finished orders = " << pCook->GetFinishedOrders() << " at TS " << CurrentTimeStep << " and should be finished at TS " << pOrd->GetFinishTime() << endl;
			continue;
		}
		else if (pOrd->GetUrgent())
		{	//if there are no cooks except for injured and in break

			if (InBreakCooks.GetCount())
			{	//if there are cooks in break
				pCook = InBreakCooks.RemoveFirst();
				pCook->SetInBreak(false);
			}
			else if (InjuredCooks.GetCount())
			{	//if there are cooks in rest
				pCook = InjuredCooks.RemoveFirst();
				pCook->SetInjured(false);
			}

			if (pCook)
			{
				WaitingVIP.dequeue(pOrd);
				pCook->SetAssignedOrder(pOrd);
				pCook->SetFinishedOrders(pCook->GetFinishedOrders() + 1);	//Incementing the number of orders that the cook has done
				pOrd->SetInServiceTime(CurrentTimeStep);
				pOrd->SetFinishTime(CurrentTimeStep + ceil(float(pOrd->GetOrderSize()) / pCook->GetSpeed()));
				pOrd->SetStatus(SRV);
				InService.InsertEnd(pOrd);
				WorkingCooks.InsertSorted(pCook);	//Returning the cook back to the working list
				cout << "\nOrder " << pOrd->GetID() << " of size " << pOrd->GetOrderSize() << " is assigned to cook " << pCook->GetID() << " with speed " << pCook->GetSpeed() << " and finished orders = " << pCook->GetFinishedOrders() << " at TS " << CurrentTimeStep << " and should be finished at TS " << pOrd->GetFinishTime() << endl;
			}
			else return; //There are neither free cooks nor in break or in rest cooks of all types so we return as other orders won't be assigned either
		}
		else return;	//There are no available cooks of all types so we return as other orders won't be assigned either
	}

	///////////////////////////////////Assigning Vegan Orders///////////////////////////////////
	while (WaitingVegan.peekFront(pOrd) && pOrd->GetArrivalTime() <= CurrentTimeStep)	//while there is vegan orders in waiting list till this current time step
	{
		pCook = !FreeVegan.IsEmpty() ? FreeVegan.RemoveFirst() : NULL; //Looks for a Vegan cook

		if (pCook)
		{
			WaitingVegan.dequeue(pOrd);
			pCook->SetAssignedOrder(pOrd);
			pCook->SetFinishedOrders(pCook->GetFinishedOrders() + 1);	//Incementing the number of orders that the cook has done
			pOrd->SetInServiceTime(CurrentTimeStep);
			pOrd->SetFinishTime(CurrentTimeStep + ceil(float(pOrd->GetOrderSize()) / pCook->GetSpeed()));
			pOrd->SetStatus(SRV);
			InService.InsertEnd(pOrd);
			WorkingCooks.InsertSorted(pCook);	//Returning the cook back to the working list
			cout << "\nOrder " << pOrd->GetID() << " of size " << pOrd->GetOrderSize() << " is assigned to cook " << pCook->GetID() << " with speed " << pCook->GetSpeed() << " and finished orders = " << pCook->GetFinishedOrders() << " at TS " << CurrentTimeStep << " and should be finished at TS " << pOrd->GetFinishTime() << endl;
			continue;
		}
		else break;		//There are no available cooks of type VGEN
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
			pCook->SetFinishedOrders(pCook->GetFinishedOrders() + 1);	//Incementing the number of orders that the cook has done
			pOrd->SetInServiceTime(CurrentTimeStep);
			pOrd->SetFinishTime(CurrentTimeStep + ceil(float(pOrd->GetOrderSize()) / pCook->GetSpeed()));
			pOrd->SetStatus(SRV);
			InService.InsertEnd(pOrd);
			WorkingCooks.InsertSorted(pCook);	//Returning the cook back to the working list
			cout << "\nOrder " << pOrd->GetID() << " of size " << pOrd->GetOrderSize() << " is assigned to cook " << pCook->GetID() << " with speed " << pCook->GetSpeed() << " and finished orders = " << pCook->GetFinishedOrders() << " at TS " << CurrentTimeStep << " and should be finished at TS " << pOrd->GetFinishTime() << endl;
			continue;
		}
		else break;		//There are no available cooks of type NRM
	}
	while (!FreeNormal.IsEmpty()) WorkingCooks.InsertSorted(FreeNormal.RemoveFirst());	//
	while (!FreeVegan.IsEmpty()) WorkingCooks.InsertSorted(FreeVegan.RemoveFirst());	//Returning the cooks back to the working list
	while (!FreeVIP.IsEmpty()) WorkingCooks.InsertSorted(FreeVIP.RemoveFirst());		//
}

void Restaurant::UpdateWaiting() //auto promote waiting nomral to vip, and waiting vip to urgent
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
		if (CurrentTimeStep - VIP_Order[i]->GetPromotionTS() >= vip_wt && !VIP_Order[i]->GetUrgent())
		{
			pOrd = VIP_Order[i];
			WaitingVIP.DeleteNode(VIP_Order[i]->GetID());
			pOrd->SetUrgent(true);
			WaitingVIP.enqueue(pOrd, pOrd->GetArrivalTime());	//make urgent orders at the first of the queue
			UrgentOrders++;
			cout << "\nOrder " << pOrd->GetID() << " turned urgent at TS " << CurrentTimeStep << " with total " << UrgentOrders << " urgent orders\n";
		}
	}

	////////////////////////////////////Autopromotion for Normal/////////////////////////////////////
	while (WaitingNormal.peekFront(pOrd) && (CurrentTimeStep - (pOrd->GetArrivalTime()) == autop))
	{
		WaitingNormal.dequeue(pOrd);
		pOrd->SetType(TYPE_VIP);
		pOrd->SetPromotionTS(CurrentTimeStep);
		float Priority = pOrd->GetOrderMoney() / (pOrd->GetOrderSize() * pOrd->GetArrivalTime());
		WaitingVIP.enqueue(pOrd, Priority);
		AutoPromoted++;
		cout << "\nOrder " << pOrd->GetID() << " is autopromoted to VIP at TS " << CurrentTimeStep << " with total " << AutoPromoted << " autopromoted orders\n";
	}
}

//////////////////////////////////  Printing simulation data at each timestep   /////////////////////////////

void Restaurant::PrintData()
{
	/////////////////////Getting the number of urgent orders in this time step/////////////////////////
	int UrgentOrders_Count = 0, n = VIP_OrdCount;
	for (int i = 0; i < n; i++)
	{
		Order* pOrd;
		WaitingVIP.dequeue(pOrd);
		if (pOrd->GetUrgent())
		{
			UrgentOrders_Count++;
			VIP_OrdCount--;
			continue;
		}
		float p = pOrd->GetOrderMoney() / float(pOrd->GetOrderSize() * pOrd->GetArrivalTime());
		WaitingVIP.enqueue(pOrd, p);
	}
	string urgent = to_string(UrgentOrders_Count);

	string waitingNRM = to_string(NRM_OrdCount);	//
	string waitingVGN = to_string(VGN_OrdCount);	//Turn waiting orders counters into strings
	string waitingVIP = to_string(VIP_OrdCount);	//

	string finishedNRM = to_string(NRM_FinishedCount);	//
	string finishedVGN = to_string(VGN_FinishedCount);	//Turn finished orders counters into strings
	string finishedVIP = to_string(VIP_FinishedCount);	//

	string C_NRM = to_string(NRM_C);	//
	string C_VGN = to_string(VGN_C);	//Turn cooks counters into strings
	string C_VIP = to_string(VIP_C);	//

	string timestep = to_string(CurrentTimeStep);

	pGUI->PrintMessage("TS: " + timestep, 1, 0);	//print current timestep
	pGUI->PrintMessage("Waiting: (Normal = " + waitingNRM + "), (Vegan = " + waitingVGN + "), (VIP = " + waitingVIP + "), (Urgent = " + urgent + ")", 2, 0);	//print waiting orders numbers
	pGUI->PrintMessage("Cooks: (Normal = " + C_NRM + "), (Vegan = " + C_VGN + "), (VIP = " + C_VIP + ")", 3, 0);		//Prints the number if cooks
	pGUI->PrintMessage("Finished: (Normal = " + finishedNRM + "), (Vegan = " + finishedVGN + "), (VIP = " + finishedVIP + ")", 5, 0);	//print finished orders numbers

	int csize = 0, i = 0;
	Cook** C_Arr = WorkingCooks.toArray(csize);
	int count = 0;
	for (int j = 0; j < csize; j++)
	{
		if (C_Arr[j]->GetAssignedOrder())
		{	//Check for assigned cooks

			string ctype;
			if (C_Arr[j]->GetType() == 0) ctype = "N";
			else if (C_Arr[j]->GetType() == 1) ctype = "G";
			else ctype = "V";

			string newFinishTime = "";
			if (C_Arr[j]->GetInjured())
			{
				ctype = "Inj" + ctype;
				newFinishTime = to_string(C_Arr[j]->GetAssignedOrder()->GetFinishTime());
			}

			char otype;
			if (C_Arr[j]->GetAssignedOrder()->GetType() == 0) otype = 'N';
			else if (C_Arr[j]->GetAssignedOrder()->GetType() == 1) otype = 'G';
			else otype = 'V';

			string C_ID = to_string(C_Arr[j]->GetID());
			string O_ID = to_string(C_Arr[j]->GetAssignedOrder()->GetID());

			//Printing data of assigned cooks
			if (newFinishTime == "")
				pGUI->PrintMessage(ctype + C_ID + "(" + otype + O_ID + ")", 4, count);
			else
				pGUI->PrintMessage(ctype + C_ID + "(" + otype + O_ID + ")[" + newFinishTime + "]", 4, count);

			count++;
		}
	}
}