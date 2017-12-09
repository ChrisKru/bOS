#pragma once
#include "Process.h"
#include <vector>
#include <iostream>
#include <queue>
#include <memory>
using namespace std;
shared_ptr<PCB> running = FirstProcess(0);
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
public:
	Scheduler()
	{
		time = 0;
	}
	void Schedule()
	{
		if (running != nullptr && running->GetState() == Stan::ZAKONCZONY)
		{
			time = running->timer;
		}
		if (running != nullptr && running->GetState() != Stan::AKTYWNY)
		 {
			if (procesy_otrzymane.size() > 0)
			{
				bool x = false;
				for (auto proces : procesy_otrzymane)
				{
					if (proces->GetState() == Stan::NOWY) proces->SetState(Stan::GOTOWY);
					if (proces->GetState() == Stan::GOTOWY)
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
				for (auto proces : procesy_gotowe)
			{
				cout << proces->ID << " - Tau: " << proces->tau << endl;
				procesy_gotowe_queue.push(proces);
			}
				procesy_otrzymane.clear();
				procesy_gotowe.clear();
				if (procesy_gotowe_queue.size() > 0)
				{
					running = procesy_gotowe_queue.top();
					procesy_gotowe_queue.pop();
					running->SetState(Stan::AKTYWNY);
					cout << "Aktywny: " << running->ID << endl;
				}
			}
			else
			{
				if (procesy_gotowe_queue.size() <= 0)
				{
					cout << "Brak gotowych procesow!" << endl;
					running = FirstProcess(0);
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
		cout << "Akutalnie wykonywany jest proces o ID: " << running->GetID() << endl;
	}
	 void dodaj_do_procesow_gotowych(shared_ptr<PCB> proces)
	{
		procesy_otrzymane.push_back(proces);
	}
	

};

