#pragma once
#include <string>
#include <queue>
#include <vector>
#include <queue>

#include "ExchangeFile.h"

using namespace std;

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
	queue<FrameInfo> FIFO;		// tablica, funkcjonuj¹ca jako kolejka FIFO.
								// zapisuje po kolei informacje o stronicach, 
								// które wchodz¹ do ramek, by móc ustaliæ któr¹
								// z nich usun¹æ gdy pamiêæ siê zape³ni
	ExchangeFile file;			// u¿ywany plik wymiany
	std::vector<PageTable> pagetables;	// vector ze wszystkimi procesami (tablicami
										// stronic), które wesz³y do pamiêci

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

	std::string getCommand(int programCounter, int PID) {
		// zwraca komendê, o któr¹ prosi interpreter
		// trzeba dodaæ j¹ do RAMu

		for (int i = 0;i < pagetables.size();i++) {
			if (pagetables[i].getID() == PID) {
				// wchodzimy do tablicy stronic procesu

				// obliczamy w której stronicy znajduje siê adres
				int page = pagetables[i].getPageNumber(programCounter);
				
				if (pagetables[i].VIBit[page] == true) {
					// w przypadku gdy ju¿ jest w RAMie:
					int frame = pagetables[i].FrameNumber[page];
					int r = 16 * page;
					std::string data;
					for (int ii = 0;ii < 16;ii++) {
						data[ii] = RAM[r];
						r++;
					}
					return data;

				}
				else {
					// w przypadku gdy stronicy jeszcze nie ma w RAMie:
					tab data_;
					data_.PID = PID;
					// pobieranie z pliku wymiany
					//for (int j = 0;j < 16;j++){
					data_ = file.getData(PID,page);
					//}

					// szukamy wolnej ramki
					if (FIFO.size() < 8) {
						for (int k = 0;k < 8;k++) {
							if (freeFrame[k] == true) {
								// po znalezieniu wolnej ramki
								int r = 16 * k;
								for (int kk = 0;kk < 16;kk++) {
									RAM[r] = data_.data[kk];
									r++;
								}
								pagetables[i].FrameNumber[page] = k; // dajemy znaæ w której ramce jest stronica
								pagetables[i].VIBit[page] = true;	// ustawiamy VIBit
								// udokumentowanie, ¿e stronica wesz³a do pamiêci
								FrameInfo newframe_;
								newframe_.FrameNumber = k;
								newframe_.PID = PID;
								newframe_.pageNumber = page;
								FIFO.push(newframe_);
								PIDinFrame[k] = PID;
								freeFrame[k] = false;

								std::string data = "";
								for (int o = 0;o < 16;o++) {
									data += data_.data[o];
								}
								//data = data_.data;
								return data;
								break;
							}
						}
					}
					else {
						// je¿eli nie znajdziemy wolnej ramki
						// musimy wyrzucic pierwsz¹, która wesz³a
						int newframe;
						newframe = FIFO.front().FrameNumber;

						// ustawienie, ¿e dane wychodz¹ z ramki
						pagetables[PIDinFrame[newframe]].FrameNumber[FIFO.front().pageNumber] = -1;
						pagetables[PIDinFrame[newframe]].VIBit[FIFO.front().pageNumber] = false;
						FIFO.pop();

						// zast¹pienie danych potrzebnymi
						int r = 16 * newframe;
						for (int l = 0;l < 16;l++) {
							RAM[r] = data_.data[l];
							r++;
						}
						// ustawienie, ¿e dane wesz³y do ramki
						pagetables[i].FrameNumber[page] = newframe;
						pagetables[i].VIBit[page] = true;
						FrameInfo nwframe;
						nwframe.FrameNumber = newframe;
						nwframe.PID = PID;
						nwframe.pageNumber = page;
						FIFO.push(nwframe);
						PIDinFrame[newframe] = PID;
						freeFrame[newframe] = false;
						
						std::string data;
						for (int u = 0;u < 16;u++) {
							data += data_.data[u];
						}
						return data;
					}
				}
			} break;
		}
	}

	void loadProcess(int PID, int size, std::string filename) {
		PageTable table(PID, size);
		table = file.DataToFile(PID, filename, size);
		pagetables.push_back(table);
	}

	void showExchangeFile() {
		file.show();
	}

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