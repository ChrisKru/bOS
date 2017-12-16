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

	std::string getCommand(int PID, int commandCounter) {
		// zwraca komend�, o kt�r� prosi interpreter
		// trzeba doda� j� do RAMu

		int first, last;
		first = file.getFirstIndex(PID, commandCounter);
		last = file.getLastIndex(PID, commandCounter);
		int lenght = last - first + 1;
		bool thesame;

		for (int i = 0;i < pagetables.size();i++) {
			if (pagetables[i].getID() == PID) {
				// wchodzimy do tablicy stronic procesu

				int page, page2 = -1;
				// sprawdzamy czy pocz�tek i koniec s� w tej samej stronicy
				if (pagetables[i].getPageNumber(first) == pagetables[i].getPageNumber(last)) {
					page = pagetables[i].getPageNumber(first);
					thesame = true;
				}
				else {
					// je�eli s� w dw�ch r�nych stronicach, to obie musimy w�o�y� do pami�ci
					page = pagetables[i].getPageNumber(first);
					page2 = pagetables[i].getPageNumber(last);
					thesame = false;
				}
				

				std::string command = "";

				// -- stronica PAGE (pierwsza) --

				if (pagetables[i].VIBit[page] == true) {
					// w przypadku gdy ju� jest w RAMie:
					int frame = pagetables[i].FrameNumber[page];
					int it = frame * 16 + (first - page*16);
					if(thesame==true){
						for (int p = 0;p < lenght;p++) {
							command += RAM[it];
							it++;
						} return command;
						break;
					}
					else {
						int endofframe = (frame+1)*16 - 1;
						int end = endofframe - it + 1;
						for (int p = 0;p < end;p++) {
							command += RAM[it];
							it++;
						}
					}
				}
				else {
					// w przypadku gdy stronicy jeszcze nie ma w RAMie:
					// �adujemy dane z pliku wymiany
					tab data_;
					data_.PID = PID;
					data_ = file.getData(PID,page);

					// szukamy wolnej ramki dla PAGE
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
					}

					int it = first - page * 16;
					if (thesame == true) {
						for (int p = 0;p < lenght;p++) {
							command += data_.data[it];
							it++;
						}
						return command;
					}
					else {
						while (it > 16) {
							command += data_.data[it];
							it++;
						}
					}
				}

				// -- stronica PAGE2 (druga) --

				if(thesame==false){
					if (pagetables[i].VIBit[page2] == true) {
						// w przypadku gdy ju� jest w RAMie:
						int frame = pagetables[i].FrameNumber[page2];
						int it = frame * 16;
						int end = frame * 16 + (last - page2 * 16 + 1);
						for (int p = 0;p < end;p++) {
							command += RAM[it];
							it++;
							}
						return command;
					}
					else {
						tab data_;
						data_.PID = PID;
						data_ = file.getData(PID, page2);

						// szukamy wolnej ramki dla PAGE2
						if (FIFO.size() < 8) {
							for (int k = 0;k < 8;k++) {
								if (freeFrame[k] == true) {
									// po znalezieniu wolnej ramki
									int r = 16 * k;
									for (int kk = 0;kk < 16;kk++) {
										RAM[r] = data_.data[kk];
										r++;
									}
									pagetables[i].FrameNumber[page2] = k; // dajemy zna� w kt�rej ramce jest stronica
									pagetables[i].VIBit[page2] = true;	// ustawiamy VIBit
																		// udokumentowanie, �e stronica wesz�a do pami�ci
									FrameInfo newframe_;
									newframe_.FrameNumber = k;
									newframe_.PID = PID;
									newframe_.pageNumber = page2;
									FIFO.push(newframe_);
									PIDinFrame[k] = PID;
									freeFrame[k] = false;
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
							pagetables[i].FrameNumber[page2] = newframe;
							pagetables[i].VIBit[page2] = true;
							FrameInfo nwframe;
							nwframe.FrameNumber = newframe;
							nwframe.PID = PID;
							nwframe.pageNumber = page2;
							FIFO.push(nwframe);
							PIDinFrame[newframe] = PID;
							freeFrame[newframe] = false;
						}
						int end = last - page2 * 16 + 1;
						for (int p = 0;p < end;p++) {
							command += data_.data[p];
						}
						return command;
					}
				}
			}
		}
	}

	void loadProcess(int PID, std::string filename) {
		// metoda �aduj�ca proces do pliku wymiany
		// konieczne jest wywo�anie tej metody zawsze wtedy gdy rusza proces

		//ustalanie wielkosci pliku - wielkosci procesu
		int size;
		std::string line, allfile;
		std::fstream txtfile;
		txtfile.open(filename);
		if (txtfile) {
			while (!txtfile.eof()) {
				std::getline(txtfile, line);
				allfile += line;
			}
		}
		size = allfile.size();

		txtfile.close();

		PageTable table(PID, size);
		table = file.DataToFile(PID, filename, size);
		pagetables.push_back(table);
	}

	void showExchangeFile() {
		//wy�wietla zawarto�� pliku wymiany
		std::cout << "PLIK WYMIANY" << std::endl;
		file.show();

		// je�li mo�na, to lepiej tego nie u�ywa�. mi�dzy znakami wyrzuca krzaczki-niewidki
	}

	void show() {
		// wy�wietla zawarto�� pami�ci
		std::cout << "RAM" << std::endl;
		std::cout << RAM << std::endl;
	}

	void showFIFO() {
		// wy�wietla zawarto�� kolejki FIFO
		
		std::queue<FrameInfo> showthis = FIFO;

		if (!showthis.empty()) {
			std::cout << "KOLEJKA FIFO" << std::endl;
			for (int i = 0;i < showthis.size();i++) {
				std::cout << "Ramka: " << showthis.front().FrameNumber << "; PID: " <<
					showthis.front().PID << "; stronica procesu: " << showthis.front().pageNumber << std::endl;
				showthis.pop();
			}
			std::cout << "Ramka: " << showthis.back().FrameNumber << "; PID: " <<
				showthis.back().PID << "; stronica procesu: " << showthis.back().pageNumber << std::endl;
		}
	}

	void showPageTable(int PID) {
		// wy�wietla zawarto�� tablicy stronic dla danego procesu
		for (int i = 0;i < pagetables.size();i++) {
			if (pagetables[i].getID() == PID) {
				pagetables[i].show();
			}
		}
	}


	// -------------------------------------- tego nie b�dzie --------------------------------------
	void getCommandRange(int PID, int counter) {
		// metoda tylko do sprawdzania czy dobrze wyznacza�em
		// przedzia�y w pliku wymiany. nie b�dzie nigdzie nigdy u�ywana

		int a, b;
		a = file.getFirstIndex(PID, counter);
		b = file.getLastIndex(PID, counter);
		std::cout << "Pocz: " << a << ", kon: " << b << std::endl;
	}
};