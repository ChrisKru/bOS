#pragma once
#include <vector>

class PageTable {
private:
	int ID;					// ID przechwyconego procesu do tablicy stronic
	int pages;				// ilo�� stronic zale�na od rozmiaru procesu
	int FrameNumber[64];	// tablica, zawieraj�ca w sobie numer ramki dla ka�dej ze stronic
	bool VIBit[64];			// bit, m�wi�cy czy dana stronica jest w pami�ci

public:
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
			FrameNumber[i] = -1;
			VIBit[i] = false;
		}
	}

	int getPositionInMemory(int LogicalAdress, int lenght) {
		// przechwytuje od interpretera adres logiczny rozkazu oraz jego d�ugo��,
		// najpierw poszukuje go w tablicy stronic, a zwraca numer ramki, w kt�rej 
		// znajduje si� dany rozkaz lub -1 gdy nie ma go w pami�ci


	}

	int getPageNumber(int MemoryAdress) {
		// zwraca numer stronicy, kt�ra znajduje si� w pami�ci pod adresem
		// MemoryAdress

		int n;
		for (int i = 0;i < pages;i++) {
			if (VIBit[i] == 1) {
				if (MemoryAdress == FrameNumber[i]) {
					n = FrameNumber[i];
				}
			}
		}
		return n;
	}

	int getPages() {
		return pages;
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