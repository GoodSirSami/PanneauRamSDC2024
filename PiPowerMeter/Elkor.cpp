#include "Elkor.h"

unsigned int calculerCRC(unsigned char *input, unsigned int len)
{
	unsigned int CRC, Low, Hi;
	unsigned int i, j;

	CRC = 0XFFFF;

	for (i = 0; i < len; i++)
	{
		CRC = input[i] ^ CRC;

		for (j = 0; j < 8; j++)
		{
			if (CRC & 0x0001)
			{ // LSB == 1
				CRC = CRC >> 1;
				CRC = CRC ^ 0xA001;
			}
			else // LSB == 0
				CRC = CRC >> 1;
		}
	}

	// il faut inverser le low et le hi byte
	Low = CRC & 0x00FF;
	Low = Low << 8;
	Hi = CRC & 0xFF00;
	Hi = Hi >> 8;
	CRC = Hi | Low;

	return CRC;
}
