#include "PageTable.h"


PageTable::PageTable(int PID, int ProcessSize) {
	ID = PID;
	if (ProcessSize % 16 == 0) {
		pages = ProcessSize / 16;
	}
	else pages = ProcessSize / 16 + 1;

	for (int i = 0;i < pages;i++) {
		// domyœlne wartoœci
		FrameNumber.push_back(-1);
		VIBit.push_back(false);
	}
}



int PageTable::getPageNumber(int LogicalAddress) {
	int p;
	p = LogicalAddress / 16;

	if (p < pages)
		return p;
	else
		return -1;
}



int PageTable::getPages() {
	return pages;
}



int PageTable::getID() {
	return ID;
}



void PageTable::show() {
	for (int i = 0;i < pages;i++) {
		std::cout << "Stronica nr " << i << ".: ";
		if (VIBit[i] == 0) std::cout << "nie ma jej w pamieci";
		if (VIBit[i] == 1) std::cout << "w pamieci pod adresem " << FrameNumber[i] << ".";
		std::cout << std::endl;
	}
}
