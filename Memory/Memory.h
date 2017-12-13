#pragma once
#include <string>
#include <queue>
#include <vector>
#include <queue>

#include "PageTable.h"
#include "ExchangeFile.h"

using namespace std;

struct FrameInfo {
	int FrameNumber;			// numer ramki
	int PID;					// ID procesu, z któego pochodzi stronica
};

class Memory {
private:
	char RAM[128];				// wirtualna pamiêæ operacyjna
	bool freeFrame[8];			// tablica ustalaj¹ce czy ramki s¹ wolne
	int PIDinFrame[8];			// zapisuje ID procesu do ramki by ³atwiej by³o
								// ustaliæ sk¹d ona siê wziê³a
	queue<FrameInfo> FIFO;		// tablica, funkcjonuj¹ca jako kolejka FIFO.
								// zapisuje po kolei informacje o stronicach, 
								// które wchodz¹ do ramek, by móc ustaliæ któr¹
								// z nich usun¹æ gdy pamiêæ siê zape³ni
	ExchangeFile file;			// u¿ywany plik wymiany

public:

	Memory() {
		// konstruktor zape³nia tablicê freeFrame wartoœciami true, oraz 
		// PIDinFrame wartoœciami -1

		for (int i = 0;i < 8;i++) {
			freeFrame[i] = true;
		}
		for (int i = 0;i < 8;i++) {
			PIDinFrame[i] = -1;
		}
	}

	void loadPage(PageTable table, int PageNumber) {
		// wpisuje do pamiêci wskazan¹ stronicê, sprawdza czy jest miejsce
		// je¿eli nie ma, to na podstawie kolejki FIFO usuwa proces, bêd¹cy
		// najd³u¿ej w pamiêci

		// NO DOBRA, ALE OD CZEGO ZALE¯Y KTÓRA STRONICA MA WEJŒÆ DO PAMIÊCI?
		// KTO MI KARZE COKOLWIEK WPISYWAÆ?

		// muszê wiedzieæ co i dok³adnie kiedy ma byæ wpisywane do pamiêci
		// argumenty funkcji te¿ mog¹ ulec zmianie

	}

	void deleteProcess(int PID) {
		// usuwa wszystkie ramki, które zawiera³y proces
		// metoda nie jest konieczna, poniewa¿ nowy proces mo¿e nadpisaæ
		// to co znajduje siê w wybranej ramce

		// ZASTANOWIÊ SIÊ NAD TYM CZY TO JEST NAPRAWDÊ KONIECZNE
	}

	std::string getCommand(); // do dopracowania

	void show() {
		// wyœwietla zawartoœæ pamiêci
		
		int lastIndex;
		std::cout << "RAM (wyswietlany ramkami)" << std::endl;
		for (int i = 0;i < 8;i++) {
			std::cout << i << ": ";
			for (int k = lastIndex;k < 16;k++) {
				std::cout << RAM[k];
				lastIndex++;
			}
		}
	}

	void showFIFO() {
		// wyœwietla zawartoœæ kolejki FIFO
	}

};