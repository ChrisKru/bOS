#include "dysk.h"


bool Disc::file_exist(std::string filename)
{
	for (int i = 0; i < 32; i++)
	{
		if (katalog_[i].filename == filename)
			return 1;
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
			return i;
	}
	return -1;
}

void Disc::available_free_space()
{
	int temp = 0;
	for (int i = 0; i < 32; i++)
	{
		if (block_[i] == true)
		{
			temp++;
		}
	}
	free_space_ = temp * 32;
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
		for (int i = 0; i < length; i++)
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



Disc::Disc()
{
	for (int i = 0; i < 1024; i++)
	{
		disc_[i] = 0;
	}
	for (int i = 0; i < 32; i++)
	{
		katalog_[i].free = true;
		block_[i] = true;
	}
}

void Disc::create_file(std::string filename)
{
	if (!file_exist(filename))
	{
		int kat_nr = free_catalog();
		int block_nr = free_block();
		katalog_[kat_nr].filename = filename;
		katalog_[kat_nr].free = false;
		katalog_[kat_nr].first_block = block_nr;
		katalog_[kat_nr].size = 0;
		block_[block_nr] = false;
		available_free_space();
	}
	else if (free_space_==0)
	{
		std::cout << "Dysk jest pelny" << std::endl;
	}
	else
	{
		std::cout << "Taki plik juz istnieje" << std::endl;
	}
}

void Disc::write_file(std::string filename, std::string data)
{
	if (!file_exist(filename))
	{
		create_file(filename);
	}

	int kat_nr = find_file(filename);

	if (kat_nr != -1)
	{
		if (katalog_[kat_nr].size == 0)
		{
			int length = data.length();
			if (length - 31 > free_block_space())
			{
				std::cout << "Dysk jest pelny" << std::endl;
			}
			else
			{
				int poczatek = katalog_[kat_nr].first_block * 32;
				save_block(poczatek, data);
				//katalog_[kat_nr].data = true;
				katalog_[kat_nr].size = length;
				available_free_space();

			}
		}
		else
		{
			std::cout << "Nie mozna nadpisac pliku" << std::endl;
		}
	}
}

void Disc::print_file(std::string filename)
{
	/*for(int i=0;i<32;i++)
	{
		if(katalog_[i].free==false)
		{

		}
	}*/
}

void Disc::delete_file(std::string filename)
{
}

void Disc::print_file_list()
{
}

void Disc::rename_file(std::string filename, std::string new_filename)
{
}

void Disc::add_to_file(std::string filename, std::string data)
{
}