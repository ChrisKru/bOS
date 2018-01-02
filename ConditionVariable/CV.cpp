#include "CV.h"

bool CV::wait() {
		if (pcb_waiting_list.size() > 0) {
			if (pcb_waiting_list.front()->GetID() != running->GetID()) {
				running->SetState(State::OCZEKUJACY);
				pcb_waiting_list.push_back(running);
				std::cout << "Plik jest juz otwarty" << std::endl;
				return false;
			}
		}
		else {
			pcb_waiting_list.push_back(running);
		}
	used = true;
	return used;
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
	else {
		used = false;
	}
}
bool CV::is_used() {
	return used;
}
void CV::set_used(bool used) {
	this->used = used;
}
void CV::remove_killed() {
	if (used) {
		if (pcb_waiting_list.front()->GetState() == State::ZAKONCZONY) {
			signal();
		}
	}
}