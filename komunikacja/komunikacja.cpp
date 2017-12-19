
#include "komunikacja.h"
#include <iostream>

//--------metody dla komuniaktu

Komunikat::Komunikat()
{

}
Komunikat::Komunikat(int nadawca, std::string tresc)
{
	id_nadawcy = nadawca;
	tresc_komunikatu = tresc;
	rozmiar_komunikatu = tresc_komunikatu.size();
}
/*
int Komunikat::getID()
{
	return id_nadawcy;
}

int Komunikat::getRozmair()
{
	return rozmiar_komunikatu;
}
*/
std::string Komunikat::getTresc()
{
	return tresc_komunikatu;
}

//---------metody dla kolejki
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
Komunikat Kolejka::receive(std::string nazwa_nadawcy)
{
	if (kolejka.size() > 0)
	{
		Komunikat odebrany = kolejka.front();	//mo�e to da� w jednej linii jednak z tym u g�ry
		usun_komunikat();	//po odczytaniu komunikatu z kolejki, musi zosta� z niej usuni�ty
		std::cout <<"Tresc odebranego komunikatu: "<< odebrany.tresc_komunikatu;
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
void Kolejka::send(int id_odbiorcy, Komunikat komunikat)
{	//dost�p do kolejki: id procesu jest potrzebne. Wykorzystuj� funkcj� GetPCB, kt�a zwraca shared pointer do PCB. Wtedy b�dzie PCB->kolejka
	//std::list<Group>ProcessGroupsList;
	std::shared_ptr<PCB> x = GetPCB(id_odbiorcy);
	int grupa = x->ProcessGroup;

	for (auto &it : ProcessGroupsList)
	{
		if (it.ProcessGroup == grupa)
		{
			for (auto &e : it.ProcessList)
			{
				if (e->ProcessID == id_odbiorcy)
					Kolejka::dodaj_komunikat(komunikat);
			}
		}

	}
}
