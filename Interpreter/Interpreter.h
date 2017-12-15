#pragma once

#include "../Memory/Memory.h"
#include "../dysk/dysk.h"
#include "../procesor/Scheduler.h"
#include "../komunikacja/kolejka.h"

#include <string>
#include <array>

/*
w runInstruction() zwracamy false, gdy nie wykonamy programu, czyli wystapia jakies bledy
*/

class Interpreter
{
public:
	int _PID;
	int _RegA;
	int _RegB;
	int _RegC;
	int _IP;
	bool _done;								// wartosc zwracana przy operacji runIstruction()
	bool _flagEQ;							// flaga ustawiana przy operacji EQ -> r�wne sobie

private:
	std::array<std::string, 3> instruction;	/* tablica zmiennych, znajduje si� w niej kolejno: 
											 [0] operacja, zawsze dwu znakowa
											 [1] (opcjonalne) dana do operacji
											 [2] (opcjonalne) dana do operacji
											 */

	void setInstruction();			// ustawiamy w nim nasza tablice instruction
	Disc dysk;
	Memory RAM;
	Scheduler scheduler;
	Kolejka komunikacja;
	

public:
	Interpreter();

	void loadRegister();			// ustawia nasze wewnetrze rejestry, PID oraz wskaznik rozkazow korzystajac z running od Scheduler
	std::string loadInstruction();	// za�adowanie rozkazu z danymi od pamieci RAM
	void saveRegisters();;			// ustawia dane zawarte w running naszymi danymi(rejestry, PID, IP, timer++)
	void showRegisters();			// wyswietla stany rejestrow, PID oraz IP

	bool runInstruction(Disc ds, Memory mm, Scheduler sc, Kolejka km);			// glowna metoda, zawiera interpretacje rozkazow

};

