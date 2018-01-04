#include "Scheduler.h"

shared_ptr<PCB> running = nullptr;
bool komparator::operator() (shared_ptr<PCB> p1, shared_ptr<PCB> p2)
{
	return p1->Tau > p2->Tau;
}
Scheduler::Scheduler() //to wywoluja wszyscy oprocz shella na poczatku w main
{
	time = 0;
}
Scheduler::Scheduler(int a) // to wywoluje Shell w main
{ 
	idle = FirstProcess(0);
	time = 0;
	id1e = make_shared<PCB>("bezczynny");
	idle->SetState(State::AKTYWNY);
	running = id1e;
}
void Scheduler::Schedule()
{
	/*if (procesy_otrzymane.size() > 0)
	{
		for (auto proces : procesy_otrzymane)
		{
			if (proces->GetState() == State::NOWY) proces->SetState(State::GOTOWY);
		}
	}*/
	if (procesy_otrzymane.size() == 0 && procesy_gotowe_queue.size() == 0 && running->GetName() == "bezczynny")
	{
		bool a = false;
		for (auto & x : ProcessGroupsList)
		{
			for (auto y : x.ProcessList)
			{
				if (y->GetState() == State::OCZEKUJACY)
				{
					y->SetState(State::GOTOWY);
					procesy_otrzymane.push_back(y);
					a = true;
					break;
				}
			}
			if (a == true) break;
		}
	}
	if (running != nullptr && (running->GetState() == State::ZAKONCZONY || running->GetState() == State::OCZEKUJACY))
	{
		time = running->Timmer;
		if (running->GetState() == State::OCZEKUJACY) running->SetTimmer(0);
		//idle->SetState(State::AKTYWNY);
	}
	if (running != nullptr && running->GetState() != State::AKTYWNY)
	{
		if (procesy_otrzymane.size() > 0)
		{
			if (procesy_gotowe_queue.size() > 0)
			{
				int size = procesy_gotowe_queue.size();
				vector<shared_ptr<PCB>> temp = procesy_otrzymane;
				procesy_otrzymane.clear();
				for (int i = 0; i < size; i++)
				{
					procesy_otrzymane.push_back(procesy_gotowe_queue.top());
					procesy_gotowe_queue.pop();
				}
				for (int i = 0; i < temp.size(); i++)
				{
					procesy_otrzymane.push_back(temp[i]);
				}
			}
			bool x = false;
			for (auto proces : procesy_otrzymane)
			{
				if (proces->GetState() == State::NOWY) proces->SetState(State::GOTOWY);
				if (proces->GetState() == State::GOTOWY)
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
				//cout << proces->ProcessName << " - Tau: " << proces->GetTau() << endl;
				procesy_gotowe_queue.push(proces);
			}
			procesy_otrzymane.clear();
			procesy_gotowe.clear();
			if (procesy_gotowe_queue.size() > 0)
			{
				running = procesy_gotowe_queue.top();
				procesy_gotowe_queue.pop();
				running->SetState(State::AKTYWNY);
				idle->SetState(State::GOTOWY);
				cout << "Przydzielono procesor: " << running->ProcessName << " PID: " << running->ProcessID << endl;// " z tau = " << running->GetTau() << endl;

			}
		}
		else //if procesy_orzymane.size() == 0
		{
			if (procesy_gotowe_queue.size() == 0) // jesli nie ma nic w kolejce
			{

				cout << "Brak gotowych procesow, ";
				running = id1e;
				idle->SetState(State::AKTYWNY);
			}
			else // jesli jest to przelicz tau i wybierz nowy proces
			{
				int siz = procesy_gotowe_queue.size();
				for (int i = 0; i < siz ; i++)
				{
					procesy_otrzymane.push_back(procesy_gotowe_queue.top());
					procesy_gotowe_queue.pop();
				}
				bool x = false;
				for (int i = 0; i < procesy_otrzymane.size(); i++)
				{
					if (x == false)
					{
						procesy_otrzymane[i]->SetTau(time*alpha + (1 - alpha)*running->GetTau());
						procesy_gotowe_queue.push(procesy_otrzymane[i]);
						x = true;
					}
					else
					{
						procesy_otrzymane[i]->SetTau(time*alpha + (1 - alpha)*procesy_otrzymane[i - 1]->GetTau());
						procesy_gotowe_queue.push(procesy_otrzymane[i]);

					}
				}
				procesy_otrzymane.clear();
				running = procesy_gotowe_queue.top();
				procesy_gotowe_queue.pop();
				running->SetState(State::AKTYWNY);
				idle->SetState(State::GOTOWY);
				cout << "Przydzielono procesor: " << running->ProcessName << " PID: " << running->ProcessID << endl;//" z tau = " << running->GetTau() << endl;
			}

		}
	}
}
void Scheduler::print_running()
{
	if (running->ProcessState == State::AKTYWNY) cout << "Akutalnie wykonywany proces o nazwie: " << running->ProcessName << " PID: " << running->ProcessID
		<< " Nr grupy: " << running->ProcessGroup << endl;
	else cout << "Nie jest wykonywany zaden proces, aktywny proces bezczynnosci" << endl;
}
void Scheduler::wyswietl_gotowe()
{
	priority_queue<shared_ptr<PCB>, vector<shared_ptr<PCB>>, komparator> procesy_gotowe_queue2 = procesy_gotowe_queue;
	if (procesy_gotowe_queue2.size() > 0 && procesy_otrzymane.size() > 0) {
		int temp = procesy_gotowe_queue2.size();
		for (int i = 0; i < temp; i++)
		{
			cout << "Nazwa: " << procesy_gotowe_queue2.top()->ProcessName << " PID: " << procesy_gotowe_queue2.top()->ProcessID 
				<< " Nr grupy: " << procesy_gotowe_queue2.top()->ProcessGroup << endl;
			procesy_gotowe_queue2.pop();
		}
		for (auto proces : procesy_otrzymane)
		{
			cout << "Nazwa: " << proces->ProcessName << " PID: " << proces->ProcessID << " Nr grupy: " << proces->ProcessGroup << endl;
		}
	}
	else if (procesy_otrzymane.size() > 0)
	{
		for (auto proces : procesy_otrzymane)
		{
			cout << "Nazwa: " << proces->ProcessName << " PID: " << proces->ProcessID << " Nr grupy: " << proces->ProcessGroup << endl;
		}
	}
	else if (procesy_gotowe_queue2.size() > 0)
	{
		int temp = procesy_gotowe_queue2.size();
		for (int i = 0; i < temp; i++)
		{
			cout << "Nazwa: " << procesy_gotowe_queue2.top()->ProcessName << " PID: " << procesy_gotowe_queue2.top()->ProcessID 
				<< " Nr grupy: " << procesy_gotowe_queue2.top()->ProcessGroup << endl;
			procesy_gotowe_queue2.pop();
		}
	}
	else
	{
		cout << "Brak procesow gotowych" << endl;
	}
}
void Scheduler::killprocess()
{
	if (running->ProcessName != "bezczynny")
	{
		running->SetState(State::ZAKONCZONY);
		DeleteProcess(running->GetID());
	}
}
void Scheduler::usun(string name)
{
	if (procesy_otrzymane.size() > 0)
	{
		procesy_otrzymane.erase(remove_if(procesy_otrzymane.begin(), procesy_otrzymane.end(),
			[name](shared_ptr<PCB> p) {return p->GetName() == name; }));
	}
	if (procesy_gotowe_queue.size() > 0)
	{
		priority_queue<shared_ptr<PCB>, vector<shared_ptr<PCB>>, komparator> a = procesy_gotowe_queue;
		priority_queue<shared_ptr<PCB>, vector<shared_ptr<PCB>>, komparator> b;
		int si = procesy_gotowe_queue.size();
		for (int i = 0; i < si; i++)
		{
			if (a.top()->GetName() == name)
			{
				a.pop();
			}
			else
			{
				shared_ptr<PCB> t = a.top();
				a.pop();
				b.push(t);
			}
		}
		procesy_gotowe_queue = b;
	}
}
void dodaj_do_procesow_gotowych(shared_ptr<PCB> proces)
{
	procesy_otrzymane.push_back(proces);
}


