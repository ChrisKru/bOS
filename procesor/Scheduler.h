#pragma once
#include "../ProcessM/ProcessM.h"
#include <vector>
#include <iostream>
#include <queue>
#include <memory>
using namespace std;
extern shared_ptr<PCB> running;
struct komparator
{
	bool operator() (shared_ptr<PCB> p1, shared_ptr<PCB> p2);
};
class Scheduler
{
	int time;
	float alpha = 0.5;
	vector<shared_ptr<PCB>> procesy_gotowe;
	priority_queue<shared_ptr<PCB>, vector<shared_ptr<PCB>>, komparator> procesy_gotowe_queue;
public:
	Scheduler();
	Scheduler(int a);
	void Schedule();
	void print_running();
	void wyswietl_gotowe();
	void killprocess();
};
void dodaj_do_procesow_gotowych(shared_ptr<PCB> proces);
