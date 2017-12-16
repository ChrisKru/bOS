
#include "../ProcessM/ProcessM.h"


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

	A, B, C = 0;

	CommandCounter = 0;
	Tau = 0;
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
void PCB::PrintPCBInformations() {};
int PCB::GetCommandCounter() {

return CommandCounter;

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
void NewProcess(std::string ProcessName, int ProcessGroup) {

	std::shared_ptr<PCB> New = std::make_shared<PCB>(ProcessName, ProcessGroup);
	for (auto it = ProcessGroupsList.begin(); it != ProcessGroupsList.end(); ++it) {
		if (ProcessGroup == it->ProcessGroup) it->ProcessList.push_back(New);
		//else NewProcessGroup(ProcessName); // nie wiem czy wyrzucaæ bl¹d gdy podasz grupê ktora nie stnieje czy odrazu utworzyæ tak¹ grupê
	}
	procesy_otrzymane.push_back(New); // funkcja niebezpieczna potrzebna dla procesora !!!
};
void DeleteProcess(int ProcessID) {
	for (auto it = ProcessGroupsList.begin(); it != ProcessGroupsList.end(); ++it) {
		it->ProcessList.remove_if([ProcessID](std::shared_ptr<PCB> n) { return n->GetID() == ProcessID; });
	}// co zrobiæ gdy usuniemy ostatni proces z danje grupy, usun¹c odrazu grupê ?
};
void DeleteProcessGroup(int ProcessGroup) {

	ProcessGroupsList.remove_if([ProcessGroup](Group n) {return n.ProcessGroup == ProcessGroup; });

};
std::shared_ptr<PCB> FirstProcess(int ProcessGroup) {

	std::shared_ptr<PCB>first = std::make_shared<PCB>("bezczynny", ProcessGroup); // nie wiej ajaki tu stan, sprawdz jak mieli w stystemie z c++

	return first;
};
void NewProcessGroup(std::string ProcessName) {//??????????????????
	Group NewGroup; // numer powinien byc adany przez nadzorcê
	NewGroup.ProcessList.push_back(FirstProcess(NewGroup.ProcessGroup));
	ProcessGroupsList.push_back(NewGroup);
};
void SetStateID(int ProcessID, State state) {

	for (auto &v : ProcessGroupsList) {
		for (auto &x : v.ProcessList) {
			x->SetState(state);
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
void PrintGroupInformation(int ProcessGroup) {
	for (auto it = ProcessGroupsList.begin(); it != ProcessGroupsList.end(); ++it) {
		if (ProcessGroup == it->ProcessGroup) {
			for (auto x : it->ProcessList) {
				std::cout << "Nazwa: " << x->GetName() << std::endl;
				std::cout << "Id: " << x->GetID() << std::endl;
				std::cout << "Stan: " << x->GetState() << std::endl;

			}
		}
	}

};
void PrintProcessListInformation() {

	for (auto &v : ProcessGroupsList) {

		std::cout << "Numer grupy:" << v.ProcessGroup << std::endl;

	}

};

