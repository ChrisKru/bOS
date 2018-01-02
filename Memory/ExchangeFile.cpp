#include "../Memory/ExchangeFile.h"


ExchangeFile::ExchangeFile() { }


PageTable ExchangeFile::DataToFile(int& PID, std::string& fileName, int& size) {
	int pages;
	std::string line, allfile;
	std::fstream txtfile;
	txtfile.open(fileName);
	if (txtfile) {
		// wpisanie wartoœci pliku txt do tymczasowego stringa
		while (!txtfile.eof()) {
			std::getline(txtfile, line);
			allfile += line + ';';
		}
	}
	txtfile.close();

	// utworzenie tablicy stronnic dla procesu
	PageTable pgtable(PID, size);
	pages = pgtable.getPages();

	// teraz tymczasow¹ tablicê podzielimy na tablice po 16 znaków i
	// wpiszemy je do pliku wymiany
	int fromfile = 0;
	for (int i = 0;i < pages;i++) {

		tab table;

		// for zapisuje pojedyncze znaki do pomocniczego chara
		for (int k = 0;k < 16;k++) {
			if (fromfile < allfile.size()) {
				table.data[k] = allfile[fromfile++];
				//fromfile++;
			}
			else {
				table.data[k] = ' ';
			}
		}
		//std::cout << table.data << std::endl; //<-- sprawdza³em czy dobrze zapisuje

		table.PID = PID;
		file.push_back(table);
		file;
		// dodanie informacji o wrzucanych danych
		OneProcess thisproc;
		thisproc.page = i;
		thisproc.PID = PID;
		container.push_back(thisproc);
	}

	return pgtable;
}



tab ExchangeFile::getData(int ProcID, int pageNumber) {
	tab returnthis;
	for (int i = 0;i < container.size();i++) {
		if ((container[i].PID == ProcID) && (container[i].page == pageNumber)) {
			for (int j = 0;j < 16;j++) {
				returnthis.data[j] = file[i].data[j];
			}
			return returnthis;
		}
	}
}



int ExchangeFile::getFirstIndex(int PID, int commandCounter) {

	if (commandCounter == 0) return 0;

	int separators = 0;
	for (int i = 0;i < file.size();i++) {
		if (file[i].PID == PID) {
			for (int j = 0;j < 16;j++) {
				if (file[i].data[j] == ';') {
					separators++;
					if (separators == commandCounter) { return (container[i].page * 16 + j + 1); break; }
				}
			}
		}
	}
}



int ExchangeFile::getLastIndex(int PID, int commandCounter) {

	int separators = 0;
	for (int i = 0;i < file.size();i++) {
		if (file[i].PID == PID) {
			for (int j = 0;j < 16;j++) {
				if (file[i].data[j] == ';') {
					separators++;
					if (separators == commandCounter + 1) { return (container[i].page * 16 + j - 1); break; }
				}
			}
		}
	}
}

void ExchangeFile::deleteProcessData(int PID) {
	for (int i = 0;i < file.size();i++) {
		if (file[i].PID == PID) {
			file.erase(file.begin() + i);
			container.erase(container.begin() + i);
			i--;
		}
	}
}

void ExchangeFile::show() {

	for (int i = 0;i < file.size();i++) {
		for (int o = 0;o < 16;o++) {
			std::cout << file[i].data[o];
		}
	}
	std::cout << std::endl;
}
