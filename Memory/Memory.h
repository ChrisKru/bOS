#pragma once
#include <string>
#include <queue>
#include <vector>
#include <queue>

#include "ExchangeFile.h"

using namespace std;

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
	queue<FrameInfo> FIFO;		// tablica, funkcjonuj�ca jako kolejka FIFO.
								// zapisuje po kolei informacje o stronicach, 
								// kt�re wchodz� do ramek, by m�c ustali� kt�r�
								// z nich usun�� gdy pami�� si� zape�ni
	ExchangeFile file;			// u�ywany plik wymiany
	std::vector<PageTable> pagetables;	// vector ze wszystkimi procesami (tablicami
										// stronic), kt�re wesz�y do pami�ci

public:

	Memory() {
		// konstruktor zape�nia tablic� freeFrame warto�ciami true, oraz 
		// PIDinFrame warto�ciami -1

		for (int i = 0;i < 8;i++) {
			freeFrame[i] = true;
		}
		for (int i = 0;i < 8;i++) {
			PIDinFrame[i] = -1;
		}
	}

	std::string getCommand(int programCounter, int PID) {
		// zwraca komend�, o kt�r� prosi interpreter
		// trzeba doda� j� do RAMu

		for (int i = 0;i < pagetables.size();i++) {
			if (pagetables[i].getID() == PID) {
				// wchodzimy do tablicy stronic procesu

				// obliczamy w kt�rej stronicy znajduje si� adres
				int page = pagetables[i].getPageNumber(programCounter);
				
				if (pagetables[i].VIBit[page] == true) {
					// w przypadku gdy ju� jest w RAMie:
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
								pagetables[i].FrameNumber[page] = k; // dajemy zna� w kt�rej ramce jest stronica
								pagetables[i].VIBit[page] = true;	// ustawiamy VIBit
								// udokumentowanie, �e stronica wesz�a do pami�ci
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
						// je�eli nie znajdziemy wolnej ramki
						// musimy wyrzucic pierwsz�, kt�ra wesz�a
						int newframe;
						newframe = FIFO.front().FrameNumber;

						// ustawienie, �e dane wychodz� z ramki
						pagetables[PIDinFrame[newframe]].FrameNumber[FIFO.front().pageNumber] = -1;
						pagetables[PIDinFrame[newframe]].VIBit[FIFO.front().pageNumber] = false;
						FIFO.pop();

						// zast�pienie danych potrzebnymi
						int r = 16 * newframe;
						for (int l = 0;l < 16;l++) {
							RAM[r] = data_.data[l];
							r++;
						}
						// ustawienie, �e dane wesz�y do ramki
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
		// wy�wietla zawarto�� pami�ci
		
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
		// wy�wietla zawarto�� kolejki FIFO
	}

};