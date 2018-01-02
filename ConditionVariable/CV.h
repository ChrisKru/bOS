#pragma once
#include <list>
#include <memory>
#include "../procesor/Scheduler.h"

class CV {
	bool used = false;
	std::list<std::shared_ptr<PCB>> pcb_waiting_list;
public:
	bool wait();
	void signal();
	bool is_used();
	void set_used(bool used);
	void remove_killed();
};