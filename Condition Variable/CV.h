#pragma once
#include "Process.h"
#include <list>
#include <memory>

class CV {
	std::list<std::shared_ptr<PCB>> pcb_waiting_list;

public:
	void wait() {
		if (pcb_waiting_list.size() == 0) {
			// nie musze nic robic
		}
		else {
			running->SetState(State::OCZEKUJACY);
			pcb_waiting_list.push_back(running);
		}
	}
	void signal() {
		pcb_waiting_list.pop_front(); // mozliwe, ze jeszcze musialbym zmienic stan tego na zakonczony(?); - niby nie

		if(pcb_waiting_list.size() > 0)
		pcb_waiting_list.front()->PCB::SetState(State::GOTOWY);
	}
	bool is_empty() {
		if (pcb_waiting_list.size() == 0) return true;
		else return false;
	}
};