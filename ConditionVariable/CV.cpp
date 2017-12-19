#include "CV.h"

void CV::wait() {
	pcb_waiting_list.remove_if([](std::shared_ptr<PCB> proces) { if(proces->GetState() == State::ZAKONCZONY) return proces; });

	if (!(pcb_waiting_list.size() == 0))
		running->SetState(State::OCZEKUJACY);

	pcb_waiting_list.push_back(running);
}
void CV::signal() {
	pcb_waiting_list.pop_front(); // mozliwe, ze jeszcze musialbym zmienic stan tego na zakonczony(?); - niby nie
	pcb_waiting_list.remove_if([](std::shared_ptr<PCB> proces) { if (proces->GetState() == State::ZAKONCZONY) return proces; });

	if (pcb_waiting_list.size() > 0)
		pcb_waiting_list.front()->PCB::SetState(State::GOTOWY);
}
bool CV::is_empty() {
	if (pcb_waiting_list.size() == 0) return true;
	else return false;
}