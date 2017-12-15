#pragma once
#include <vector>
#include <iostream>

class PageTable {
private:
	int ID;							// ID przechwyconego procesu do tablicy stronic
	int pages;						// ilo�� stronic zale�na od rozmiaru procesu
public:
	std::vector<int> FrameNumber;	// tablica, zawieraj�ca w sobie numer ramki dla ka�dej ze stronic
	std::vector<bool> VIBit;		// bit, m�wi�cy czy dana stronica jest w pami�ci

	PageTable(int PID, int ProcessSize) {
		// konstruktor przechwytuje ID procesu, tworzy dla niego tablic� stronic,
		// oblicza ile potrzeba stronic dla procesu, ustawia domy�lne warto�ci
		// tablicom FrameNumber oraz VIBit

		ID = PID;
		if (ProcessSize % 16 == 0) {
			pages = ProcessSize / 16;
		}
		else pages = ProcessSize / 16 + 1;
		
		for (int i = 0;i < pages;i++) {
			FrameNumber.push_back(-1);
			VIBit.push_back(false);
		}
	}

	int getPageNumber(int LogicalAddress) {
		// zwraca stronic�, w kt�rej powinien znajdowa� si� wskazany adres

		int p;
		p = LogicalAddress / 16;
		
		if (p < pages)
			return p;
		else
			return -1;
	}

	int getPages() {
		return pages;
	}

	int getID() {
		return ID;
	}

	void show() {
		// wyswietla zawartosc tablicy stronic

		std::cout << "TABLICA STRONIC (dla procesu o ID: " << ID << ")" << std::endl;
		for (int i = 0;i < pages;i++) {
			std::cout << "Stronica nr " << i << ".: ";
			if (VIBit[i] == 0) std::cout << "nie ma jej w pamieci";
			if (VIBit[i] == 1) std::cout << "w pamieci pod adresem " << FrameNumber[i] << ".";
			std::cout << std::endl;
		}
	}

};