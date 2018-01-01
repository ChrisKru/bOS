#pragma once
#include <queue>

#include "ExchangeFile.h"

struct FrameInfo {
	int FrameNumber;			// numer ramki
	int PID;					// ID procesu, z któego pochodzi stronica
	int pageNumber;				// numer stronicy tego procesu
};

class Memory {
private:
	char RAM[128];				// wirtualna pamiêæ operacyjna
	bool freeFrame[8];			// tablica ustalaj¹ce czy ramki s¹ wolne
	int PIDinFrame[8];			// zapisuje ID procesu do ramki by ³atwiej by³o
								// ustaliæ sk¹d ona siê wziê³a
	std::queue<FrameInfo> FIFO;	// tablica, funkcjonuj¹ca jako kolejka FIFO.
								// zapisuje po kolei informacje o stronicach, 
								// które wchodz¹ do ramek, by móc ustaliæ któr¹
								// z nich usun¹æ gdy pamiêæ siê zape³ni
	ExchangeFile file;			// u¿ywany plik wymiany
	std::vector<PageTable> pagetables;	// vector ze wszystkimi procesami (tablicami
										// stronic), które wesz³y do pamiêci

public:

	Memory();
		// konstruktor zape³nia tablicê freeFrame wartoœciami true, oraz 
		// PIDinFrame wartoœciami -1

	std::string getCommand(int PID, int commandCounter);
		// zwraca komendê, o któr¹ prosi interpreter
		// trzeba dodaæ j¹ do RAMu

	bool loadProcess(int PID, std::string filename);
		// metoda ³aduj¹ca proces do pliku wymiany
		// konieczne jest wywo³anie tej metody zawsze wtedy gdy rusza proces

	void show();
		// wyœwietla zawartoœæ pamiêci

	void showExchangeFile();
		//wyœwietla zawartoœæ pliku wymiany
		// jeœli mo¿na, to lepiej tego nie u¿ywaæ. miêdzy znakami wyrzuca krzaczki-niewidki

	void showFIFO();
		// wyœwietla zawartoœæ kolejki FIFO

	void showPageTable(int PID);
		// wyœwietla zawartoœæ tablicy stronic dla danego procesu

	void getCommandRange(int PID, int counter);
		// metoda tylko do sprawdzania czy dobrze wyznacza³em
		// przedzia³y w pliku wymiany

		// nie bêdzie nigdzie nigdy u¿ywana, ale zostawiam bo mog¹
		// przytrafiæ siê jakieœ poprawki
};