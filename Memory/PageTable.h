#pragma once
#include <vector>
#include <iostream>

class PageTable {
private:
	int ID;							// ID przechwyconego procesu do tablicy stronic
	int pages;						// iloœæ stronic zale¿na od rozmiaru procesu
	std::vector<int> FrameNumber;	// tablica, zawieraj¹ca w sobie numer ramki dla ka¿dej ze stronic
	std::vector<bool> VIBit;		// bit, mówi¹cy czy dana stronica jest w pamiêci

public:
	PageTable(int PID, int ProcessSize) {
		// konstruktor przechwytuje ID procesu, tworzy dla niego tablicê stronic,
		// oblicza ile potrzeba stronic dla procesu, ustawia domyœlne wartoœci
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
		// przechwytuje od interpretera adres logiczny rozkazu oraz jego d³ugoœæ,
		// najpierw poszukuje go w tablicy stronic, a zwraca numer ramki, w której 
		// znajduje siê dany rozkaz lub -1 gdy nie ma go w pamiêci

		// MO¯E OKAZAÆ SIÊ NIEPOTRZEBNA, KWESTIÊ WSPÓ£PRACY Z INTERPRETEREM POZOSTAJ¥ DO USTALENIA
	}

	int getPageNumber(int MemoryAdress) {
		// zwraca numer stronicy, która znajduje siê w pamiêci pod adresem
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