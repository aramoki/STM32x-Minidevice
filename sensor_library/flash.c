#include "flash.h"
#include <string.h>
#include "resetseeker.h"

FLASH_Status flashsil(int sector)
{
	FLASH_Status error;
	//0x08010000
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	error = FLASH_ErasePage(sector);
	while (error != FLASH_COMPLETE)
	{
	}
	//FLASH_Lock();
	return error;
}

FLASH_Status flashyaz(uint32_t address, uint32_t data)
{
	int a = 0xffffffff;
	FLASH_Status error;

	FLASH_Unlock();

	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	//FLASH_EraseOptionBytes();

	error = FLASH_ProgramWord(address, data);
	while (error != FLASH_COMPLETE)
	{
		if (a-- == 0)
		{
			break;
		}
	}

	//FLASH_Lock();
	return error;
}

FLASH_Status flashyaz_adr(int adr_num, uint32_t data)
{
	uint32_t start = 0x08010000;
	start += 32 * adr_num;

	return flashyaz(start, data);
}

FLASH_Status flashyaz_name(int adr_num, char name[31])
{
	uint32_t start = 0x08010000;
	uint32_t str32;
	int i;
	start += 1024 * adr_num + 4;

	str32 = 0x43444546;

	for (i = 0; i < 31; i += 4)
	{

		if (name[i] == 0x00)
		{
			str32 = 0;
			flashyaz(start + i, str32);
			break;
		}
		else if (name[i + 1] == 0x00)
		{
			str32 = (name[i]);
			flashyaz(start + i, str32);
			break;
		}
		else if (name[i + 2] == 0x00)
		{
			str32 = (name[i]) | (name[i + 1] << 8);
			flashyaz(start + i, str32);
			break;
		}
		else if (name[i + 3] == 0x00)
		{
			str32 = (name[i]) | (name[i + 1] << 8) | (name[i + 2] << 16);
			flashyaz(start + i, str32);
			break;
		}
		else
		{
			str32 = (name[i]) | (name[i + 1] << 8) | (name[i + 2] << 16) | (name[i + 3] << 24);
			flashyaz(start + i, str32);
		}
	}
}

FLASH_Status flashyaz_val(int adr_num, uint16_t temp, uint16_t hum)
{
	uint32_t start = 0x08010000;
	uint32_t str32;
	int i;
	start += 32 * adr_num + 16;
	str32 = hum | (temp << 16);
	flashyaz(start, str32);
}

void oku_name(int adr_num)
{
	uint32_t start = 0x08010000;
	int tt = gettimer();
	char name[33];
	int i, j;
	start += 1024 * adr_num + 4;

	cmd(0xc0);
	for (i = 0; i < 31; i++)
	{
		name[i] = *(__IO uint8_t *)(start++);
		if (name[i] == 0x00)
		{
			name[i] = 0;
			break;
		}
		else
		{
			//data(name[i]);
		}
	}
	if (strlen(name) > 16)
	{

		string1(&name[(tt) % strlen(name)]);
		string1("___");
	}
	else
	{
		string1(name);
		for (j = 0; j < 16 - strlen(name); j++)
		{
			data(' ');
		}
	}

	// bazen ekrana fazladan HARF basiyor onuda full 16 nulle cozersin
}

uint16_t oku_hum(int adr_num)
{
	uint32_t start = 0x08010000;
	char name[16];
	start += 32 * adr_num + 16;

	return *(__IO uint16_t *)(start);
}

uint16_t oku_temp(int adr_num)
{
	uint32_t start = 0x08010000;
	char name[16];
	start += 32 * adr_num + 16 + 2;

	return *(__IO uint16_t *)(start);
}
