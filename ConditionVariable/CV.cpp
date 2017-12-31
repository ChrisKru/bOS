#include "CV.h"

bool CV::wait() {
	bool temp = false;

	if (pcb_waiting_list.size() > 0) {
		if (pcb_waiting_list.front()->GetState() == State::ZAKONCZONY) {
			temp = true;
			signal();
		}
		pcb_waiting_list.remove_if([](std::shared_ptr<PCB> proces) {return proces->GetState() == State::ZAKONCZONY; });

		if (pcb_waiting_list.size() > 0) {
			if (pcb_waiting_list.front()->GetID() != running->GetID()) {
				running->SetState(State::OCZEKUJACY);
				pcb_waiting_list.push_back(running);
			}
		}else pcb_waiting_list.push_back(running);
	}
	else {
		pcb_waiting_list.push_back(running);
	}
	return temp;
}
void CV::signal() {
	pcb_waiting_list.pop_front();
	pcb_waiting_list.remove_if([](std::shared_ptr<PCB> proces) {return proces->GetState() == State::ZAKONCZONY; });

	if (pcb_waiting_list.size() > 0) {
		if (pcb_waiting_list.front()->GetState() == State::OCZEKUJACY) {
			pcb_waiting_list.front()->PCB::SetState(State::GOTOWY);
			dodaj_do_procesow_gotowych(pcb_waiting_list.front());
		}
	}
}
bool CV::is_empty() {
	if (pcb_waiting_list.size() == 0)
		return true;
	else
		return false;
}