
#include "../ProcessM/ProcessM.h"

int GroupNumber = 0;
OverseerID IDs;
std::list<Group>ProcessGroupsList;
std::vector<std::shared_ptr<PCB>> procesy_otrzymane;

OverseerID::OverseerID() {

	for (int i = 0; i < LimitID; i++) {
		FreeIDList.push_back(1);
	}
}

int OverseerID::GetFreeID() {

	for (int i = 0; i < LimitID; i++) {
		if (FreeIDList[i] == 1) {
			FreeID = i;
			FreeIDList[i] = 0;
			return FreeID;

		}
	}

}

void OverseerID::ClearID(int ID) {
	FreeIDList[ID] = 1;
}

PCB::PCB(std::string processname, int processgroup) {

	ProcessID = IDs.GetFreeID();
	ProcessState = NOWY;
	ProcessName = processname;
	ProcessGroup = processgroup;

	A = 0;
	B = 0;
	C = 0;
	CommandCounter = 0;
	Tau = 5;
	Timmer = 0;


};
int PCB::GetID() {

	return ProcessID;

};
std::string PCB::GetName() {

	return ProcessName;

};
State PCB::GetState() {

	return ProcessState;

};
void PCB::SetState(State state) {

ProcessState = state;

};
void PCB::SetFileName(std::string filename) {

	FileName = filename;

};
void PCB::PrintPCBInformations() {};
int PCB::GetCommandCounter() {

return CommandCounter;

};

int PCB::GetProcessGroup() {

	return ProcessGroup;

};

void PCB::SetCommandCounter(int commandcounter) {

	CommandCounter = commandcounter;

};
int PCB::GetTau() {

	return Tau;

};
void PCB::SetTau(int tau) {

	Tau = tau;

};
int PCB::GetTimmer() {

	return Timmer;

};
void PCB::SetTimmer(int timmer) {

	Timmer = timmer;

};
Group::Group() {
	ProcessGroup = GroupNumber;
	GroupNumber++; // uproszcony nadzorca
};
std::shared_ptr<PCB> NewProcess(std::string ProcessName, int ProcessGroup) {
	bool f = false;
	std::shared_ptr<PCB> New;
	for (auto it = ProcessGroupsList.begin(); it != ProcessGroupsList.end(); ++it) {
		if (ProcessGroup == it->ProcessGroup) {
			New = std::make_shared<PCB>(ProcessName, ProcessGroup);
			it->ProcessList.push_back(New); f = true;
		}
	}

	if (f == false) {
		std::cout << "Podany nr grupy nie istnieje" << std::endl;
		return New = NULL;
	}
	else {
		procesy_otrzymane.push_back(New); // funkcja niebezpieczna potrzebna dla procesora !!!
		return New;
	}
};
void DeleteProcess(int ProcessID) {
	int i;
	bool f = false;
	for (auto it = ProcessGroupsList.begin(); it != ProcessGroupsList.end(); ++it) {
		it->ProcessList.remove_if([ProcessID](std::shared_ptr<PCB> n) { return n->GetID() == ProcessID; });
		if (it->ProcessList.size() == 0) {
			i = it->ProcessGroup;
			f = true;
		}
	}
	if (f == true) {
		DeleteProcessGroup(i);
	}
};
void DeleteProcessName(std::string ProcessName){
	for (auto it = ProcessGroupsList.begin(); it != ProcessGroupsList.end(); ++it) {
		it->ProcessList.remove_if([ProcessName](std::shared_ptr<PCB> n) { return n->GetName() == ProcessName; });
	}// co zrobi� gdy usuniemy ostatni proces z danje grupy, usun�c odrazu grup� ?
};
void DeleteProcessGroup(int ProcessGroup) {

	ProcessGroupsList.remove_if([ProcessGroup](Group n) {return n.ProcessGroup == ProcessGroup; });

};
std::shared_ptr<PCB> FirstProcess(int ProcessGroup) {
	Group NewGroup;
	std::shared_ptr<PCB>first = std::make_shared<PCB>("bezczynny", 0); // nie wiej ajaki tu stan, sprawdz jak mieli w stystemie z c++ !!!
	NewGroup.ProcessList.push_back(first); 
	ProcessGroupsList.push_back(NewGroup);
	return first;
};
void NewProcessGroup(std::string ProcessName) {//??????????????????
	Group NewGroup; // numer powinien byc adany przez nadzorc�
	NewGroup.ProcessList.push_back(FirstProcess(NewGroup.ProcessGroup));
	ProcessGroupsList.push_back(NewGroup);
	if (NewGroup.ProcessGroup > 0) { std::cout << "Stworzono grupe nr: " << NewGroup.ProcessGroup << std::endl; }
};
std::shared_ptr<PCB> NewProcessGroupProcess(std::string ProcessName) {//??????????????????
	Group NewGroup; // numer powinien byc adany przez nadzorc�
	ProcessGroupsList.push_back(NewGroup);
	std::shared_ptr<PCB> New = NewProcess(ProcessName, NewGroup.ProcessGroup);
	if (NewGroup.ProcessGroup > 0) { std::cout << "Stworzono grupe z procesem nr: " << NewGroup.ProcessGroup << std::endl;}
	return New;
};

void SetStateID(int ProcessID, State state) {

	for (auto &v : ProcessGroupsList) {
		for (auto &x : v.ProcessList) {
			x->SetState(state);
		}
	}

};
void SetFileNameID(int ProcessID, std::string filename) {

	for (auto &v : ProcessGroupsList) {
		for (auto &x : v.ProcessList) {
			x->SetFileName(filename);
		}
	}

};

State GetStateID(int ProcessID) {

	for (auto &v : ProcessGroupsList) {
		for (auto &x : v.ProcessList) {
			if (x->GetID() == ProcessID) return x->GetState();
		}
	}

};
std::shared_ptr<PCB> GetPCB(int ProcessID) {


	for (auto &v : ProcessGroupsList) {
		for (auto &x : v.ProcessList) {
			if (x->GetID() == ProcessID) return x;
		}
	}

};
void PrintGroupInformation() {
	for (auto it = ProcessGroupsList.begin(); it != ProcessGroupsList.end(); ++it) {
		std::cout << "Grupa nr: " << it->ProcessGroup << std::endl;
			for (auto x : it->ProcessList) {
				std::cout << "	Nazwa: " << x->GetName() << std::endl;
				std::cout << "	Id: " << x->GetID() << std::endl;
				std::cout << "	Stan: " << x->GetState() << std::endl;
				std::cout << std::endl;
			}
		
	}

};
void PrintProcessListInformation() {

	for (auto &v : ProcessGroupsList) {

		std::cout << "Numer grupy: " << v.ProcessGroup << std::endl;

	}

};

