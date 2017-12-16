
#include "komunikacja.h"

//--------metody dla komuniaktu

Komunikat::Komunikat()
{

}
Komunikat::Komunikat(std::string nadawca, std::string tresc)
{
	nazwa_nadawcy = nadawca;
	tresc_komunikatu = tresc;
	rozmiar_komunikatu = tresc_komunikatu.size();
}

std::string Komunikat::getName()
{
	return nazwa_nadawcy;
}
int Komunikat::getRozmair()
{
	return rozmiar_komunikatu;
}
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
void Kolejka::send(std::string nazwa_odbiorcy, Komunikat komunikat)
{	//dost�p do kolejki: id procesu jest potrzebne. Wykorzystuj� funkcj� GetPCB, kt�a zwraca shared pointer do PCB. Wtedy b�dzie PCB->kolejka
	//std::list<Group>ProcessGroupsList;
	int grupa = PCB::ProcessGroup;

	for (auto &it : ProcessGroupsList)
	{
		if (it.ProcessGroup == grupa)
		{
			for (auto &e : it.ProcessList)
			{
				if (e->ProcessName == nazwa_odbiorcy)
					Kolejka::dodaj_komunikat(komunikat);
			}
		}

	}
}
