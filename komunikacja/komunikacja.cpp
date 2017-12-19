
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
		Komunikat odebrany = kolejka.front();	//mo¿e to daæ w jednej linii jednak z tym u góry
		usun_komunikat();	//po odczytaniu komunikatu z kolejki, musi zostaæ z niej usuniêty
		std::cout <<"Tresc odebranego komunikatu: "<< odebrany.tresc_komunikatu;
		return odebrany;
	}
	else if (kolejka.size() == 0)
		std::cout << "Kolejka komunikatow dla tego procesu jest pusta" << std::endl;
	//jeœli ju¿ bêdê robi³ tak, ¿e Komunikat bêdzie wskaŸnikiem, to tutaj zwrócê nullptr, bo kompilator sie sra, ¿e nic tutaj na razie nie jest zwracane
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
			std::cout << e.getTresc() << std::endl;		//chyba powinno wyœwietliæ, nie mam jeszcze pewnoœci
		}
	}
}
void Kolejka::send(int id_odbiorcy, Komunikat komunikat)
{	//dostêp do kolejki: id procesu jest potrzebne. Wykorzystujê funkcjê GetPCB, któa zwraca shared pointer do PCB. Wtedy bêdzie PCB->kolejka
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
