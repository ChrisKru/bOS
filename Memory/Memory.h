#pragma once
#include <queue>

#include "ExchangeFile.h"

struct FrameInfo {
	int FrameNumber;			// numer ramki
	int PID;					// ID procesu, z kt�ego pochodzi stronica
	int pageNumber;				// numer stronicy tego procesu
};

class Memory {
private:
	char RAM[128];				// wirtualna pami�� operacyjna
	bool freeFrame[8];			// tablica ustalaj�ce czy ramki s� wolne
	int PIDinFrame[8];			// zapisuje ID procesu do ramki by �atwiej by�o
								// ustali� sk�d ona si� wzi�a
	std::queue<FrameInfo> FIFO;	// tablica, funkcjonuj�ca jako kolejka FIFO.
								// zapisuje po kolei informacje o stronicach, 
								// kt�re wchodz� do ramek, by m�c ustali� kt�r�
								// z nich usun�� gdy pami�� si� zape�ni
	ExchangeFile file;			// u�ywany plik wymiany
	std::vector<PageTable> pagetables;	// vector ze wszystkimi procesami (tablicami
										// stronic), kt�re wesz�y do pami�ci

public:

	Memory();
		// konstruktor zape�nia tablic� freeFrame warto�ciami true, oraz 
		// PIDinFrame warto�ciami -1

	std::string getCommand(int PID, int commandCounter);
		// zwraca komend�, o kt�r� prosi interpreter
		// trzeba doda� j� do RAMu

	bool loadProcess(int PID, std::string filename);
		// metoda �aduj�ca proces do pliku wymiany
		// konieczne jest wywo�anie tej metody zawsze wtedy gdy rusza proces

	void show();
		// wy�wietla zawarto�� pami�ci

	void showExchangeFile();
		//wy�wietla zawarto�� pliku wymiany
		// je�li mo�na, to lepiej tego nie u�ywa�. mi�dzy znakami wyrzuca krzaczki-niewidki

	void showFIFO();
		// wy�wietla zawarto�� kolejki FIFO

	void showPageTable(int PID);
		// wy�wietla zawarto�� tablicy stronic dla danego procesu

	void getCommandRange(int PID, int counter);
		// metoda tylko do sprawdzania czy dobrze wyznacza�em
		// przedzia�y w pliku wymiany

		// nie b�dzie nigdzie nigdy u�ywana, ale zostawiam bo mog�
		// przytrafi� si� jakie� poprawki
};