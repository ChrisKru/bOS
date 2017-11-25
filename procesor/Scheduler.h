#pragma once
#include "Process.h"
#include <vector>
#include <iostream>
#include <queue>
#include <memory>
using namespace std;
shared_ptr<PCB> running = make_shared<PCB>();
struct komparator
{
	bool operator() (shared_ptr<PCB> p1, shared_ptr<PCB> p2) 
	{
		return p1->tau > p2->tau;
	}
};
class Scheduler
{
	int time;
	float alpha = 0.5;
	vector<shared_ptr<PCB>> procesy_gotowe;
	priority_queue<shared_ptr<PCB>, vector<shared_ptr<PCB>>, komparator> procesy_gotowe_queue;
	shared_ptr<PCB> idle = nullptr;
public:
	Scheduler()
	{
		time = 0;
		running = FirstProcess();
	}
	void Schedule()
	{
		if (running != idle && running->GetState() == Stan::ZAKONCZONY)
		{
			time = running->timer;
		}
		if (running != idle && running->GetState() != Stan::AKTYWNY)
		 {
			if (procesy_otrzymane.size() > 0)
			{
				bool x = false;
				for (auto proces : procesy_otrzymane)
				{
					if (proces->GetState() == Stan::AKTYWNY)
					{
						if (time == 0)
						{
							procesy_gotowe.push_back(proces);
						}
						else
						{
							if (x == false)
							{
								proces->SetTau(time*alpha + (1 - alpha)*running->GetTau());
								procesy_gotowe.push_back(proces);
								x = true;
							}
							else
							{
								proces->SetTau(time*alpha + (1 - alpha)*procesy_gotowe.back()->GetTau());
								procesy_gotowe.push_back(proces);
							}
						}
					}
				}
			/*	for (auto proces : procesy_gotowe)
			{
				cout << proces->ID << "Tau: " << proces->tau << endl;
				procesy_gotowe_queue.push(proces);
			}*/
				procesy_otrzymane.clear();
				procesy_gotowe.clear();
				running = procesy_gotowe_queue.top();
				procesy_gotowe_queue.pop();
				running->SetState(Stan::AKTYWNY);
				cout << "Aktywny: " << running->ID << endl;
			}
			else
			{
				if (procesy_gotowe_queue.size() <= 0)
				{
					cout << "Brak procesow" << endl;
					running = FirstProcess();
				}
				else
				{
					running = procesy_gotowe_queue.top();
					procesy_gotowe_queue.pop();
					running->SetState(Stan::AKTYWNY);
					cout << "Aktywny: " << running->ID << endl;
				}
				
			}
		}
	}
	void print_running()
	{
		
	}
	 void dodaj_do_listy(shared_ptr<PCB> proces)
	{
		procesy_otrzymane.push_back(proces);
	}
	

};

