#include "armenu.h"
#include "flash.h"
#include <string.h>
#include "lcd_display.h"
#include <stdio.h>
#include <math.h>
#include "resetseeker.h"
#include "sht11.h"

float SHT11_temp, SHT11_humm;
unsigned int notcalc_temp, notcalc_hum;
char sequence[] = {' ',
				   'a', 'b', 'c', 'd', 'e', 'f', '*',
				   'g', 'h', 'i', 'j', 'k', 'l', '*',
				   'm', 'n', 'o', 'p', 'r', 's', '*',
				   't', 'u', 'v', 'y', 'z', 'w', '*',
				   'x', 'q', '1', '2', '3', '4', '*',
				   '5', '6', '7', '8', '9', '0', '*'};

char st1[10], st2[10], st3[10];
int x, y, z;

int menupage = 0;
int selected_record = -1;
int starttime = 0;

void devmenu(int seek, int press)
{
	x = 0x08010000 + seek * 4;
	sprintf(st1, "%x", x);
	sprintf(st2, "%x", *(__IO uint32_t *)(x));
	sprintf(st3, "%x", *(__IO uint32_t *)(x));

	satir1(st1);
	satir2(st2);
	satir4(st3);
	data('-');
	sayi2(seek);
	satir3("Reading");

	if (press)
	{
		menupage = 0;
	}
}

void mainmenu(int seek, int press)
{
	char *menu = "                KAYITLAR        OKUMA YAP       TEMIZLE       USB AKTAR ";
	char substr[16];
	int seeker;

	seeker = seek;
	if (seeker < 0)
	{
		seeker = -seeker;
	}

	if (seeker % 4 == 0)
	{
		satir1("Kayit            ");
	}
	else if (seeker % 4 == 1)
	{
		satir1("Olcum            ");
	}
	else if (seeker % 4 == 2)
	{
		satir1("Aktarma          ");
	}
	else if (seeker % 4 == 3)
	{
		satir1("Test             ");
	}
	satir2("Menu:");
	sayi22((seeker % 4) * 100);

	if (press)
	{

		menupage = seeker % 4 + 1;
	}
}

char name_tosave[33] = "";
int cursor = 0;

void isimkaydet(int seek, int press, int sector)
{
	int seeker = seek;

	int mintab = 0;
	char nam[5] = "Name:";
	if (cursor > 8)
	{
		mintab = cursor - 8;
	}
	if (seeker < 0)
	{
		seeker = -seeker;
	}

	satir1(nam);
	if (mintab > 5)
	{
		data('.');
		data('.');
	}
	string1(&name_tosave[mintab]);
	data('_');

	if (strlen(name_tosave) < 7)
	{
		string1("       ");
	}

	cmd(0xc0);
	sayi22(sector * 100);
	data(':');
	data(sequence[seeker % 43]);
	string1("      ");
	cmd(0xc0 + 10);
	string1("[*:ok]");

	if (press)
	{
		name_tosave[cursor] = sequence[seeker % 43];
		cursor++;
		clear();
		if (sequence[seeker % 43] == '*')
		{
			name_tosave[--cursor] = 0x00;
			flashsil(0x08010000 + sector * 1024);
			flashyaz_name(sector, name_tosave);
			flashyaz(0x08010000 + sector * 1024, 0xffffabcd);
			menupage = 6;
			starttime = gettimer();
			selected_record = sector;
			resetseek();
			clear();
			strcpy(name_tosave, "                              ");
			cursor = 0;
		}
	}
}
int kayit_submenu = 0;
int selectedone = -1;
int scalar;
int statefirstbyte;
int holdonmenu;

void kayitmenu(int seek, int press, int *seekptr)
{
	uint32_t humandtemp;
	uint16_t tt, hh;
	int temp, humm;

	if (seek >= 0)
	{

		if (kayit_submenu == 0)
		{
			scalar = seek % 64;
			statefirstbyte = *(__IO uint8_t *)(0x08010000 + scalar * 1024);
		}

		humandtemp = *(__IO uint32_t *)(0x08010000 + scalar * 1024 + 400);
		tt = humandtemp;
		hh = (humandtemp >> 16);
		temp = (int)(calc_sth11_temp(tt) * 10);
		humm = (int)(calc_sth11_humi(hh, tt) * 10);

		sprintf(st2, "%x", statefirstbyte);
		sprintf(st1, "%x", tt);
		sprintf(st3, "%x", hh);

		cmd(0x80);
		sayi22(scalar * 100);

		if (kayit_submenu == 0)
		{
			if (statefirstbyte == 0xff)
			{
				string1(" Empty          ");
				satir2("Click to create ");
				selectedone = scalar;
			}
			else
			{
				string1(" Full  ");
				sayi22(temp * 10);
				data('t');
				data(' ');
				sayi22(humm * 10);
				data('h');
				oku_name(scalar);
			}
		}
		else if (kayit_submenu == 1)
		{
			if (seek % 3 == 0)
			{
				satir2("[sil] dzn  geri ");
			}
			else if (seek % 3 == 1)
			{
				satir2(" sil [dzn] geri ");
			}
			else if (seek % 3 == 2)
			{
				satir2(" sil  dzn [geri]");
			}
		}
		else
		{
			kayit_submenu = 0;
		}

		if (press)
		{
			if (statefirstbyte == 0xff)
			{
				menupage = 5;
				kayit_submenu = 0;
			}
			else
			{

				if (kayit_submenu == 0)
				{
					*seekptr = 2;
					kayit_submenu = 1;
					holdonmenu = scalar;
				}
				else
				{

					if (seek % 3 == 0)
					{
						flashsil(0x08010000 + scalar * 1024);
						kayit_submenu = 0;
						*seekptr = holdonmenu;
					}
					else if (seek % 3 == 1)
					{
						//
					}
					else if (seek % 3 == 2)
					{
						kayit_submenu = 0;
						*seekptr = holdonmenu;
					}
				}
			}

		}
	}
	else
	{

		satir1("Ana menuye don  ");
		satir2("<--             ");
		if (press)
		{
			menupage = 0;
		}
	}
}

void gerisay()
{
	int time = gettimer() - starttime;

	satir1("Time:");
	sayi22(2500 - time * 100);
	satir2("Kayit: ");
	sayi22(selected_record * 100);
	if (time == 25)
	{
		clear();
		menupage = 7;
		starttime = gettimer();
	}
}

uint32_t records[100];

void olcumlerikaydet()
{
	int i;
	int avarage;
	uint32_t address;

	address = 0x08010000 + 1024 * selected_record + 32;

	for (i = 0; i < 10; i++)
	{
		cmd(0x80);
		satir1("Kaydediliyor %");
		sayi22(i * 100);
		avarage += records[i];
		delay_ms(100);
		flashyaz(address + i * 4, records[i]);
		cmd(0xc0 + i * 16 / 100);
		data(0xff);
	}

	avarage = avarage / 100;
	flashyaz(address + 400, avarage);

	clear();
	menupage = 0;
}

void olcumal()
{
	int time = gettimer() - starttime;
	Read_SHT11(&SHT11_temp, &SHT11_humm, &notcalc_temp, &notcalc_hum);
	satir1("H:%");
	sayi2((int)(SHT11_humm * 100));
	data(' ');
	string1("T:");
	sayi2((int)(SHT11_temp * 100));

	satir2("Kayit: ");
	sayi22(time * 50);

	if (time < 200)
	{
		records[time / 2] = (notcalc_hum << 16) | notcalc_temp;
	}
	else
	{
		menupage = 8;
		clear();
	}

}

void showmenu(int seek, int press, int *seekptr)
{
	int i;
	int firstempty;

	if (menupage == 0)
	{
		mainmenu(seek, press);
	}
	else if (menupage == 1)
	{
		kayitmenu(seek, press, seekptr);
	}
	else if (menupage == 2)
	{
		firstempty = -1;
		for (i = 0; i < 64; i++)
		{
			if (*(__IO uint8_t *)(0x08010000 + i * 1024) == 0xff)
			{
				firstempty = i;
				break;
			}
		}

		if (firstempty == -1)
		{
			satir1("Hafiza Dolu     ");
			satir2("[!] [!] [!]     ");

			if (press)
			{
				clear();
				menupage = 0;
			}
		}
		else
		{
			isimkaydet(seek, press, firstempty);
		}
	}
	else if (menupage == 3)
	{
		//
	}
	else if (menupage == 4)
	{
		devmenu(seek, press);
	}
	else if (menupage == 5)
	{
		isimkaydet(seek, press, selectedone);
	}
	else if (menupage == 6)
	{
		gerisay();
	}
	else if (menupage == 7)
	{
		olcumal();
	}
	else if (menupage == 8)
	{
		olcumlerikaydet();
	}

}