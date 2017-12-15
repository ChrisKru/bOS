#include "komunikat.h"
Komunikat::Komunikat()
{

}
Komunikat::Komunikat(int id, std::string tresc)
{
	id_nadawcy = id;
	tresc_komunikatu = tresc;
	rozmiar_komunikatu = tresc_komunikatu.size();
}

int Komunikat::getId()
{
	return id_nadawcy;
}
int Komunikat::getRozmair()
{
	return rozmiar_komunikatu;
}
std::string Komunikat::getTresc()
{
	return tresc_komunikatu;
}

