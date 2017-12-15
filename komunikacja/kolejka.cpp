#include "kolejka.h"
#include <iostream>
#include <deque>
#include <list>


Kolejka::Kolejka()
{

}
void Kolejka::dodaj_komunikat(Komunikat komunikat)
{
	kolejka.push_back(komunikat);

}
void Kolejka::usun_komunikat()
{
	if (kolejka.size() == 0)
	{
		std::cout << "Kolejka komunikatow dla tego procesu jest pusta" << std::endl;
	}
	else 
	kolejka.pop_front();
}
Komunikat Kolejka::odbierz_komunikat()
{
	if (kolejka.size() > 0)
	{
		Komunikat odebrany=kolejka.front();	//mo�e to da� w jednej linii jednak z tym u g�ry
		usun_komunikat();	//po odczytaniu komunikatu z kolejki, musi zosta� z niej usuni�ty
		return odebrany;
	}
	else if (kolejka.size() == 0)
		std::cout << "Kolejka komunikatow dla tego procesu jest pusta" << std::endl;
		//je�li ju� b�d� robi� tak, �e Komunikat b�dzie wska�nikiem, to tutaj zwr�c� nullptr, bo kompilator sie sra, �e nic tutaj na razie nie jest zwracane
}
void Kolejka::wyswietl()
{
	if (kolejka.size() == 0)
		std::cout << "Kolejka komunikatow dla tego procesu jest pusta" << std::endl;
	else
	{
		std::cout << "Kolejka zawiera nastepujace komunikaty: " << std::endl;
		for (auto e : kolejka)
		{
			std::cout << e.getTresc() << std::endl;		//chyba powinno wy�wietli�, nie mam jeszcze pewno�ci
		}
	}
}
/*
	 void Kolejka::wyslij_komunikat(int id_odbiorcy, Komunikat komunikat)
	{
		for (auto e : lista_PCB)
		{
			if (id_odbiorcy == e.ajdi)
			{
				Kolejka::dodaj_komunikat(komunikat);
			}
		}
	}
	*/
