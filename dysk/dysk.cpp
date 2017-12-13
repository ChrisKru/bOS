#include "dysk.h"


bool Disc::file_exist(std::string filename)
{
	for (int i = 0; i < 32; i++)
	{
		if (katalog_[i].filename == filename)
			return true;
	}
	return false;
}

int Disc::free_catalog()
{
	for (int i = 0; i < 32; i++)
	{
		if (katalog_[i].free == true)
			return i;
	}
	return -1;
}

int Disc::free_block()
{
	for (int i = 0; i < 32; i++)
	{
		if (block_[i] == true)
		{
			block_[i] = false;
			disc_[i * 32 + 31] = 'k';
			return i;
		}

	}
	return -1;
}

int Disc::find_file(std::string filename)
{
	for (int i = 0; i < 32; i++)
	{
		if (katalog_[i].filename == filename)
			return i;
	}
	return -1;
}

void Disc::save_block(int poczatek, std::string data)
{
	int length = data.length();
	if (length <= 31)
	{
		int j = poczatek;
		for (int i = 0; i < length; i++)
		{
			disc_[j] = data[i];
			j++;
		}
		disc_[poczatek + 31] = 'k';
	}
	else if (length >= 32)
	{
		int j = poczatek;
		for (int i = 0; i < 31; i++)
		{
			disc_[j] = data[i];
			j++;
		}

		int next_block = free_block();
		block_[next_block] = false;
		disc_[poczatek + 31] = next_block;

		data.erase(data.begin(), data.begin() + 31);
		save_block(next_block * 32, data);
	}
}

int Disc::free_block_space()
{
	int temp = 0;
	for (int i = 0; i < 32; i++)
	{
		if (block_[i] == true)
			temp++;
	}
	return temp * 31;
}

void Disc::delete_block(int nr_bloku)
{
	int next_block = 0;
	int poczatek = nr_bloku * 32;
	block_[nr_bloku] = true;
	if (disc_[poczatek + 31] != 'k')
	{
		next_block = disc_[poczatek + 31];
		delete_block(next_block);
	}
	for (int i = poczatek; i < poczatek + 32; i++)
	{
		disc_[i] = '0';
	}

}

std::string Disc::print_block(int nr_bloku, int znaki)
{
	std::string temp = "";
	int j = nr_bloku * 32;
	if (znaki <= 31)
	{
		for (int i = 0; i < znaki; i++)
		{
			temp += disc_[j];
			j++;
		}
	}
	else
	{
		for (int i = 0; i < 31; i++)
		{
			temp += disc_[j];
			j++;
		}
		temp += print_block(disc_[nr_bloku * 32 + 31], znaki - 31);
	}
	return temp;
}



Disc::Disc()
{
	for (int i = 0; i < 1024; i++)
	{
		disc_[i] = '0';
	}
	for (int i = 0; i < 32; i++)
	{
		katalog_[i].free = true;
		block_[i] = true;
	}
}

void Disc::open_file(std::string filename)
{
	if (file_exist(filename))
	{
		int kat_nr = find_file(filename);

		katalog_[kat_nr].cv_.wait();

		if (katalog_[kat_nr].open == true)
		{
			std::cout << "Plik jest juz otwarty" << std::endl;
		}
		else
		{
			katalog_[kat_nr].open = true;
		}
	}
	else
	{
		std::cout << "Plik nie istnieje" << std::endl;
	}
}

void Disc::close_file(std::string filename)
{
	if (file_exist(filename))
	{
		int kat_nr = find_file(filename);

		katalog_[kat_nr].cv_.signal();

		katalog_[kat_nr].open = false;

	}
	else
	{
		std::cout << "Plik nie istnieje" << std::endl;
	}
}

void Disc::create_file(std::string filename)
{
	int kat_nr = free_catalog();
	int block_nr = free_block();

	if (!file_exist(filename))
	{
		if (kat_nr != -1 && block_nr != -1)
		{
			katalog_[kat_nr].filename = filename;
			katalog_[kat_nr].free = false;
			katalog_[kat_nr].first_block = block_nr;
			katalog_[kat_nr].size = 0;
			block_[block_nr] = false;
		}
		else
		{
			std::cout << "Dysk jest pelny" << std::endl;
		}
	}
	else
	{
		std::cout << "Taki plik juz istnieje" << std::endl;
	}
}

void Disc::write_file(std::string filename, std::string data)
{
	if (file_exist(filename))
	{
		int kat_nr = find_file(filename);

		if (kat_nr != -1)
		{
			if (katalog_[kat_nr].size == 0)
			{
				int length = data.length();
				if (length - 31 > free_block_space())
				{
					std::cout << "Brak miejsca na dysku na zapisanie tego pliku" << std::endl;
				}
				else
				{
					int poczatek = katalog_[kat_nr].first_block * 32;
					save_block(poczatek, data);
					katalog_[kat_nr].size = length;
				}
			}
			else
			{
				std::cout << "Nie mozna nadpisac pliku" << std::endl;
			}
		}
	}
	else
	{
		std::cout << "Plik nie istnieje" << std::endl;
	}


}

void Disc::print_file(std::string filename)
{
	int kat_nr = find_file(filename);
	if (kat_nr != -1)
	{
		std::cout << getFile(filename) << std::endl;
	}
	else
	{
		std::cout << "Plik nie istnieje" << std::endl;
	}
}

void Disc::delete_file(std::string filename)
{
	if (file_exist(filename))
	{
		int kat_nr = find_file(filename);

		delete_block(katalog_[kat_nr].first_block);

		katalog_[kat_nr].free = true;
		katalog_[kat_nr].size = 0;
		katalog_[kat_nr].filename = "";
	}
	else
	{
		std::cout << "Podany plik nie istnieje" << std::endl;
	}
}

void Disc::print_file_list()
{
	std::cout << "Lista wszystkich plikow:" << std::endl;
	for (int i = 0; i < 32; i++)
	{
		if (katalog_[i].free == false)
		{
			std::cout << katalog_[i].filename << std::endl;
		}
	}
}

void Disc::rename_file(std::string filename, std::string new_filename)
{
	int kat_nr = find_file(filename);
	if (kat_nr != -1)
	{
		katalog_[kat_nr].filename = new_filename;
		std::cout << "Zmieniono nazwe pliku \"" << filename << "\" na \"" << new_filename << "\"" << std::endl;
	}
	else
	{
		std::cout << "Nie ma pliku o podanej nazwie" << std::endl;
	}
}

void Disc::add_to_file(std::string filename, std::string data)
{
	int kat_nr = find_file(filename);
	if (kat_nr != -1)
	{
		if (katalog_[kat_nr].size == 0)
		{
			std::cout << "Plik jest pusty nie mozna dopiasc plikow, uzyj innej funkcji" << std::endl;
		}
		else
		{
			if (data.size() == 0)
			{
				std::cout << "Brak danych do zapisania" << std::endl;
			}
			else
			{
				int l = katalog_[kat_nr].size % 31;
				if (l == 0)
				{
					l = 31;
				}
				int free_space = free_block_space() + (31 - l);
				if (data.size() <= free_space)
				{

					int temp_size = katalog_[kat_nr].size;
					int next_block = katalog_[kat_nr].first_block;
					while (temp_size >= 31)
					{
						if (disc_[next_block * 32 + 31] == 'k')
						{
							int temp = free_block();
							disc_[next_block * 32 + 31] = temp;
							next_block = temp;
						}
						else
						{
							next_block = disc_[next_block * 32 + 31];
						}
						temp_size -= 31;
					}
					int j = next_block * 32 + temp_size;
					int end = j + (data.size() % 31);
					int i = 0;
					for (i; j < end; i++)
					{
						if (disc_[j] == 'k')
						{
							end = j;
							break;
						}
						disc_[j] = data[i];
						j++;
					}

					katalog_[kat_nr].size += data.size();
					data.erase(data.begin(), data.begin() + i);
					if (data.size() > 0)
					{
						int next = free_block();
						disc_[end] = next;
						save_block(next * 32, data);
					}

				}
				else
				{
					std::cout << "Brak miejsca na dysku" << std::endl;
				}
			}
		}
	}
	else
	{
		std::cout << "Plik nie istnieje" << std::endl;
	}
}

std::string Disc::getFile(std::string filename)
{
	std::string temp = "";
	int kat_nr = find_file(filename);
	if (kat_nr != -1)
	{
		temp += print_block(katalog_[kat_nr].first_block, katalog_[kat_nr].size);
		std::cout << std::endl;
	}
	else
	{
		std::cout << "Plik nie istnieje" << std::endl;
	}
	return temp;
}

void Disc::printDisc()
{
	std::cout << "Zawartosc dysku (danego bloku):" << std::endl;
	int indeks = 0;
	for (int i = 0; i < 32; i++)
	{
		std::cout << i << ":\t";
		for (int j = 0; j < 32; j++)
		{
			if (j == 31)
			{
				std::cout << "\tNastepny blok: \t";
				if (disc_[indeks + j] == 'k')
				{
					std::cout << disc_[indeks + j];
				}
				else
				{
					int temp = disc_[indeks + j];
					std::cout << temp;
				}
			}
			else
			{
				std::cout << disc_[indeks + j];
			}
		}
		indeks += 32;
		std::cout << std::endl;
	}
}
