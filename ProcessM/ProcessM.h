#pragma once
#include "../komunikacja/komunikacja.h"
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <memory>

/*Stany proces�w, przy wyswietlaniu ich pokazuj� si�
odpowiednio numery kolejnisci 0, 1, 2, ...*/

enum State { NOWY, GOTOWY, ZAKONCZONY, AKTYWNY, OCZEKUJACY };


/*Przek�adanie State na nazwy zrozumiale*/

//extern std::vector<std::string> StateName = { "nowy", "gotowy", "zakonczony", "aktywny", "oczekujacy" };

extern int GroupNumber;

/*Nazdorca Id - on nadaje procesom Id w spos�w unikatowy
i nie powtarzalny, dzia�� na zasadzie tablicy zaj�tych numer�w
oraz miejsca gdzie numer nie jest zajety*/


struct OverseerID {

	int LimitID = 100;
	std::vector<int> FreeIDList;
	int FreeID;


	OverseerID();

	int GetFreeID();

	void ClearID(int ID);

};

/*Musimy miec 1 obiekt globalny dla wszytskich Id*/
extern OverseerID IDs;

/*Blok Kontrony procesu*/
//Jesli potrzebujecie jeszcze jakies POLA, GETERY, SETERY piszcie

struct PCB {


	int ProcessID;
	State ProcessState;
	std::string ProcessName;
	int ProcessGroup;

	//

	int A;
	int B;
	int C;

	//

	int CommandCounter;
	int Tau;
	int Timmer;

	// pole dla komunikacji

	Kolejka kolejka;

	// nazwa otwartego pliku

	std::string FileName;
	// Konstruktor tworz�cy nowy PCB
	PCB(std::string processname);
	PCB(std::string processname, int processgroup);

	//Metody - gdyz nie bedziecie mieli dost�pu bezpo�redio do p�l

	int GetID();
	std::string GetName();
	State GetState();
	void SetState(State state);
	void PrintPCBInformations();
	int GetCommandCounter();
	int GetProcessGroup();
	void SetCommandCounter(int commandcounter);
	int GetTau();
	void SetTau(int tau);
	int GetTimmer();
	void SetTimmer(int timmer);
	void SetFileName(std::string FileName);
};

/*Obiekt pozwalaj�cy na wsadzenie mi grup do listy (zrobienie listy grup)*/

struct Group {

	int ProcessGroup;
	Group();
	/*Jedna grupa posaida sw�j nr, nadawany przez nadzorce
	oraz list� proces�w nalez�cej do tej grupy*/
	std::list<std::shared_ptr<PCB>>ProcessList;

};

/*Globalna lista grup na kt�rej bedzie dzia�ac zarz�danie procesami*/

extern std::list<Group>ProcessGroupsList;

/*Dla procesora lista nowych proces�w*/

extern std::vector<std::shared_ptr<PCB>> procesy_otrzymane;


/*Tworzenie nowego procesu i dodawanie go do istniejacej grupy,
Id nadaje nadzorca*/
std::shared_ptr<PCB> NewProcess(std::string ProcessName, int ProcessGroup);

/*Usuwanie procesu znj�c tylko jego Id*/
void DeleteProcess(int ProcessID);

/*Usuwanie procesu znj�c tylko jego Name*/

void DeleteProcessName(std::string ProcessName);

/*Usuwanie calej grupybproces�w, bez wzgl�du na to czy jaki� jest w �rodku*/
void DeleteProcessGroup(int ProcessGroup);

/*Tworzenie pierwszego procesu przy tworzeniu grupy tzw.procesu bezczynnego*/
std::shared_ptr<PCB> FirstProcess(int ProcessGroup);

/*Tworzenie nowej grupy proces�w i odd razu rozpoczynanie w niej
procesu bezczynnego, koniecznego do funkcjonowania grupy*/
void NewProcessGroup(std::string ProcessName);

/*Ustawianie Stanu tylko po Id bez znajomosci grupy*/
void SetStateID(int ProcessID, State state);

/*Ustawianie nazwy pliku tylko po Id bez znajomosci grupy*/
void SetFileNameID(int ProcessID, std::string FileName);

/*Zwracanie Stanu tylko po Id bez znajomosci grupy*/
State GetStateID(int ProcessID);

/*Zwracanie wska�nika do procesu, kt�ry pozwala wywo�ac bezpo�rednio metody PCB*/
std::shared_ptr<PCB> GetPCB(int ProcessID);

/*Wypisywanie przyk��dowych info o ca�ej grupie proces�w
(czyli o wszytskich procesach znajduj�cyzch sie w niej)*/
void PrintGroupInformation();

/*Wypisanie info o Liscie grup proces�w*/
void PrintProcessListInformation();

/*Tworzenie grupy z dzialajacym procesem*/

std::shared_ptr<PCB> NewProcessGroupProcess(std::string ProcessName);
void NameStane(State state);
std::shared_ptr<PCB> NewProcessGroupProcessNumer(std::string ProcessName, int numer);
