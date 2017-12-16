#include "CV.h"

void CV::wait() {
	if (pcb_waiting_list.size() == 0) {
		// nie musze nic robic
	}
	else {
		running->SetState(State::OCZEKUJACY);
	}
	pcb_waiting_list.push_back(running);
}
void CV::signal() {
	pcb_waiting_list.pop_front(); // mozliwe, ze jeszcze musialbym zmienic stan tego na zakonczony(?); - niby nie

	if (pcb_waiting_list.size() > 0)
		pcb_waiting_list.front()->PCB::SetState(State::GOTOWY);
}
bool CV::is_empty() {
	if (pcb_waiting_list.size() == 0) return true;
	else return false;
}