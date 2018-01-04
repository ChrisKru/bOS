#include "../Memory/Memory.h"


Memory::Memory() {
	for (int i = 0;i < 8;i++) {
		freeFrame[i] = true;
	}
	for (int i = 0;i < 8;i++) {
		PIDinFrame[i] = -1;
	}
	for (int i = 0;i < 128;i++) {
		RAM[i] = ' ';
	}
}



std::string Memory::getCommand(int PID, int commandCounter) {
	int first, last;
	first = file.getFirstIndex(PID, commandCounter);
	last = file.getLastIndex(PID, commandCounter);
	int lenght = last - first + 1;
	bool thesame;

	for (int i = 0;i < pagetables.size();i++) {
		if (pagetables[i].getID() == PID) {
			// wchodzimy do tablicy stronic procesu

			int page, page2 = -1;
			// sprawdzamy czy pocz¹tek i koniec s¹ w tej samej stronicy
			if (pagetables[i].getPageNumber(first) == pagetables[i].getPageNumber(last)) {
				page = pagetables[i].getPageNumber(first);
				thesame = true;
			}
			else {
				// je¿eli s¹ w dwóch ró¿nych stronicach, to obie musimy w³o¿yæ do pamiêci
				page = pagetables[i].getPageNumber(first);
				page2 = pagetables[i].getPageNumber(last);
				thesame = false;
			}


			std::string command = "";

			// -- stronica PAGE (pierwsza) --

			if (pagetables[i].VIBit[page] == true) {
				// w przypadku gdy ju¿ jest w RAMie:
				int frame = pagetables[i].FrameNumber[page];
				int it = frame * 16 + (first - page * 16);
				if (thesame == true) {
					for (int p = 0;p < lenght;p++) {
						command += RAM[it];
						it++;
					} return command;
					break;
				}
				else {
					int endofframe = (frame + 1) * 16 - 1;
					int end = endofframe - it + 1;
					for (int p = 0;p < end;p++) {
						command += RAM[it];
						it++;
					}
				}
			}
			else {
				// w przypadku gdy stronicy jeszcze nie ma w RAMie:
				// ³adujemy dane z pliku wymiany
				tab data_;
				data_.PID = PID;
				data_ = file.getData(PID, page);

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
					int ProcID = PIDinFrame[newframe];
					for (int i = 0;i < pagetables.size();i++) {
						if (pagetables[i].getID() == ProcID) {
							pagetables[i].FrameNumber[FIFO.front().pageNumber] = -1;
							pagetables[i].VIBit[FIFO.front().pageNumber] = false;
							break;
						}
					}
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
					while (it < 16) {
						command += data_.data[it];
						it++;
					}
				}
			}

			// -- stronica PAGE2 (druga) --

			if (thesame == false) {
				if (pagetables[i].VIBit[page2] == true) {
					// w przypadku gdy ju¿ jest w RAMie:
					int frame = pagetables[i].FrameNumber[page2];
					int it = frame * 16;
					int end = /*frame * 16 + (*/last - page2 * 16 + 1/*)*/;
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
								pagetables[i].FrameNumber[page2] = k; // dajemy znaæ w której ramce jest stronica
								pagetables[i].VIBit[page2] = true;	// ustawiamy VIBit
																	// udokumentowanie, ¿e stronica wesz³a do pamiêci
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
						// je¿eli nie znajdziemy wolnej ramki
						// musimy wyrzucic pierwsz¹, która wesz³a
						int newframe;
						newframe = FIFO.front().FrameNumber;

						// ustawienie, ¿e dane wychodz¹ z ramki
						int ProcID = PIDinFrame[newframe];
						for (int i = 0;i < pagetables.size();i++) {
							if (pagetables[i].getID() == ProcID) {
								pagetables[i].FrameNumber[FIFO.front().pageNumber] = -1;
								pagetables[i].VIBit[FIFO.front().pageNumber] = false;
								break;
							}
						}
						FIFO.pop();
						

						// zast¹pienie danych potrzebnymi
						int r = 16 * newframe;
						for (int l = 0;l < 16;l++) {
							RAM[r] = data_.data[l];
							r++;
						}
						// ustawienie, ¿e dane wesz³y do ramki
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

void Memory::loadProcess(int PID, std::string filename) {
	//ustalanie wielkosci pliku - wielkosci procesu
	int size;
	std::string line, allfile;
	std::fstream txtfile;
	txtfile.open(filename);

	while (!txtfile.eof()) {
		std::getline(txtfile, line);
		allfile += line + ";";
	}
	size = allfile.size();

	txtfile.close();

	PageTable table(PID, size);
	table = file.DataToFile(PID, filename, size);
	pagetables.push_back(table);

}

void Memory::deleteProcess(int PID) {
	file.deleteProcessData(PID);
}

int Memory::getAddress() {
	// przeszukiwanie 
	int it = 0;
	for (int k = 0;k < 8;k++) {
		for (int j = 0;j < 16;j++) {
			if (j < 15) {
				if (RAM[it] == ' '&&RAM[it + 1] == ' '&&RAM[it + 2] == ' ') {
					if (!freeFrame[k]) {
						return it;
						break;
					}
					else {
						// zape³nianie nowej ramki
						freeFrame[k] = false;
						PIDinFrame[k] = -2;
						FrameInfo _frame;
						_frame.FrameNumber = k;
						_frame.PID = -2;
						_frame.pageNumber = 0;
						FIFO.push(_frame);
						return it;
						break;
					}
				}
			}
			it++;
		}
	}
	int newframe;
	newframe = FIFO.front().FrameNumber;
	int ProcID = PIDinFrame[newframe];
	for (int i = 0;i < pagetables.size();i++) {
		if (pagetables[i].getID() == ProcID) {
			pagetables[i].FrameNumber[FIFO.front().pageNumber] = -1;
			pagetables[i].VIBit[FIFO.front().pageNumber] = false;
			break;
		}
	}
	FIFO.pop();
	
	int address = newframe * 16;
	int k = address;
	for (int i = 0;i < 16;i++) {
		RAM[k] = ' ';
		k++;
	}

	PIDinFrame[newframe] = -2;
	freeFrame[newframe] = false;
	FrameInfo nwframe;
	nwframe.FrameNumber = newframe;
	nwframe.PID = -2;
	nwframe.pageNumber = 0;
	FIFO.push(nwframe);
	return address;
}

void Memory::writeToMemory(int LogicalAddress, std::string s) {
	int stringsize = s.size();
	if (stringsize > 3) {
		std::cout << "Wynik wychodzi poza zakres." << std::endl;
	}
	else {
		for (int i = 0;i < stringsize;i++) {
			RAM[LogicalAddress] = s[i];
			LogicalAddress++;
		}
	}
}

std::string Memory::readFromMemory(int LogicalAddress) {
	std::string ret;
	for (int i = 0;i < 3;i++) {
		ret += RAM[LogicalAddress];
		LogicalAddress++;
	}
	return ret;
}

void Memory::show() {
	int it = 0;
	for (int i = 0;i < 8;i++) {
		std::cout << "Ramka " << i << ":";
		for (int j = 0;j < 16;j++) {
			std::cout << RAM[it++];
		}
		std::cout << std::endl;
	}
}

void Memory::showExchangeFile() {
	file.show();
}

void Memory::showFIFO() {
	std::queue<FrameInfo> showthis = FIFO;
	int size = showthis.size();

	if (!showthis.empty()) {
		for (int i = 0;i < size;i++) {
			if (showthis.front().PID != -2) {
				std::cout << "Ramka: " << showthis.front().FrameNumber << "; PID: " <<
					showthis.front().PID << "; stronica procesu: " << showthis.front().pageNumber << std::endl;
				showthis.pop();
			}
			else {
				std::cout << "Ramka: " << showthis.front().FrameNumber << "; nie nalezaca do zadnego procesu"
					<< std::endl;
				showthis.pop();
			}
		}
	}
}

void Memory::showPageTable(int PID) {
	bool exist = false;
	for (int i = 0;i < pagetables.size();i++) {
		if (pagetables[i].getID() == PID) {
			pagetables[i].show();
			exist = true;
			break;
		}
	}
	if (!exist) {
		std::cout << "Brak tablicy stronic dla procesu" << std::endl;
	}
}