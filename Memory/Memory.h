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
	int PID;						// ID procesu, z kt�ego pochodzi stronica
};

class Memory {
private:
	char RAM[128];				// wirtualna pami�� operacyjna
	bool freeFrame[8];			// tablica ustalaj�ce czy ramki s� wolne
	int PIDinFrame[8];			// zapisuje ID procesu do ramki by �atwiej by�o
								// ustali� sk�d ona si� wzi�a
	queue<FrameInfo> FIFO;		// tablica, funkcjonuj�ca jako kolejka FIFO.
								// zapisuje po kolei informacje o stronicach, 
								// kt�re wchodz� do ramek, by m�c ustali� kt�r�
								// z nich usun�� gdy pami�� si� zape�ni
	ExchangeFile file;			// u�ywany plik wymiany

public:

	Memory() {
		// konstruktor zape�nia tablic� freeFrame warto�ciami true, oraz 
		// processIDinFrame warto�ciami -1
	}

	void loadPage(PageTable table, int PageNumber) {
		// wpisuje do pami�ci wskazan� stronic�, sprawdza czy jest miejsce
		// je�eli nie ma, to na podstawie kolejki FIFO usuwa proces, b�d�cy
		// najd�u�ej w pami�ci
	}

	void deleteProcess(int PID) {
		// usuwa wszystkie ramki, kt�re zawiera�y proces
		// metoda nie jest konieczna, poniewa� nowy proces mo�e nadpisa�
		// to co znajduje si� w wybranej ramce
	}

	std::string getCommand(); // do dopracowania

	void show() {
		// wy�wietla zawarto�� pami�ci
	}

	void showFIFO() {
		// wy�wietla zawarto�� kolejki FIFO
	}

};