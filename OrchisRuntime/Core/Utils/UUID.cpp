#include "UUID.h"
#include <random>
#include <iostream>
#include <bitset>
namespace Orchis
{
	static std::random_device rdm_d;
	static std::mt19937_64 rdm(rdm_d());

	static auto toNum = [&](char c) -> char {
		switch (c)
		{
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'A': return 10;
		case 'B': return 11;
		case 'C': return 12;
		case 'D': return 13;
		case 'E': return 14;
		case 'F': return 15;
		default: return 0;
		}
	};

	static auto toHex = [&](char num) -> char {
		switch (num)
		{
		case 0: return '0';
		case 1: return '1';
		case 2: return '2';
		case 3: return '3';
		case 4: return '4';
		case 5: return '5';
		case 6: return '6';
		case 7: return '7';
		case 8: return '8';
		case 9: return '9';
		case 10: return 'A';
		case 11: return 'B';
		case 12: return 'C';
		case 13: return 'D';
		case 14: return 'E';
		case 15: return 'F';
		default: return '0';
		}
	};


	// Follows RFC4122 - UUID from Truly Random or Pseudo-Random Numbers
	UUID::UUID()
	{
		m_Uuid[0] = rdm();
		m_Uuid[1] = rdm();

		//Set the two most significant bits (bits 6 and 7) of the 
		// clock_seq_hi_and_reserved to zero and one, respectively.
		*((char*)&m_Uuid[1] + 7) = *((char*)&m_Uuid[1] + 7) & ~(1 << 6);
		*((char*)&m_Uuid[1] + 7) |= 1 << 7;
		
		//Set the four most significant bits(bits 12 through 15) of the
		//	time_hi_and_version field to the 4 - bit version number from
		//	Section 4.1.3. (0     1     0     0)
		*((char*)&m_Uuid[0] + 7) = *((char*)&m_Uuid[0] + 7) & ~(1 << 7);
		*((char*)&m_Uuid[0] + 7) |= 1 << 6;
		*((char*)&m_Uuid[0] + 7) = *((char*)&m_Uuid[0] + 7) & ~(1 << 5);
		*((char*)&m_Uuid[0] + 7) = *((char*)&m_Uuid[0] + 7) & ~(1 << 4);

		//Set all the other bits to randomly (or pseudo-randomly) chosen values.
		// (Done at the beggining)
	}

	UUID::UUID(uint64_t a, uint64_t b)
	{
		m_Uuid[0] = a;
		m_Uuid[1] = b;
	}

	UUID UUID::Generate()
	{
		return UUID();
	}

	UUID UUID::FromString(const std::string& uuid)
	{
		return UUID::FromString(std::string_view(uuid));
	}

	UUID UUID::FromString(std::string_view uuid)
	{
		if (uuid[8] != '-' || uuid[13] != '-' || uuid[14] != '4' || uuid[18] != '-' || uuid[23] != '-')
			return UUID::Null();

		uint64_t a = 0, b = 0;

		*((char*)&a) |= toNum(uuid[7]);
		*((char*)&a) |= toNum(uuid[6]) << 4;

		*((char*)&a+1) |= toNum(uuid[5]);
		*((char*)&a+1) |= toNum(uuid[4]) << 4;

		*((char*)&a+2) |= toNum(uuid[3]);
		*((char*)&a+2) |= toNum(uuid[2]) << 4;

		*((char*)&a+3) |= toNum(uuid[1]);
		*((char*)&a+3) |= toNum(uuid[0]) << 4;

		*((char*)&a + 4) |= toNum(uuid[12]);
		*((char*)&a + 4) |= toNum(uuid[11]) << 4;

		*((char*)&a + 5) |= toNum(uuid[10]);
		*((char*)&a + 5) |= toNum(uuid[9]) << 4;

		*((char*)&a + 6) |= toNum(uuid[17]);
		*((char*)&a + 6) |= toNum(uuid[16]) << 4;

		*((char*)&a + 7) |= toNum(uuid[15]);
		*((char*)&a + 7) |= toNum(uuid[14]) << 4;

		*((char*)&b) |= toNum(uuid[20]);
		*((char*)&b) |= toNum(uuid[19]) << 4;

		*((char*)&b + 1) |= toNum(uuid[22]);
		*((char*)&b + 1) |= toNum(uuid[21]) << 4;

		*((char*)&b + 2) |= toNum(uuid[25]);
		*((char*)&b + 2) |= toNum(uuid[24]) << 4;
		*((char*)&b + 3) |= toNum(uuid[27]);
		*((char*)&b + 3) |= toNum(uuid[26]) << 4;
		*((char*)&b + 4) |= toNum(uuid[29]);
		*((char*)&b + 4) |= toNum(uuid[28]) << 4;
		*((char*)&b + 5) |= toNum(uuid[31]);
		*((char*)&b + 5) |= toNum(uuid[30]) << 4;
		*((char*)&b + 6) |= toNum(uuid[33]);
		*((char*)&b + 6) |= toNum(uuid[32]) << 4;
		*((char*)&b + 7) |= toNum(uuid[35]);
		*((char*)&b + 7) |= toNum(uuid[34]) << 4;

		return UUID(a, b);
	}

	const char* UUID::ToString() const
	{
		//time-low "-" time-mid "-" time-high-and-version "-" clock-seq-and-reserved clock-seq-low "-" node
		char* uuid = new char[37];
		memset(uuid, '0', 36);
		uuid[36] = 0;
		

		uuid[8] = '-';
		uuid[13] = '-';
		uuid[18] = '-';
		uuid[23] = '-';

		//time-low
		uuid[7] = toHex(*((char*)&m_Uuid[0]) & 0b1111);
		uuid[6] = toHex((*((char*)&m_Uuid[0]) >> 4) & 0b1111);
		uuid[5] = toHex(*((char*)&m_Uuid[0] + 1) & 0b1111);
		uuid[4] = toHex((*((char*)&m_Uuid[0] + 1) >> 4) & 0b1111);
		uuid[3] = toHex(*((char*)&m_Uuid[0] + 2) & 0b1111);
		uuid[2] = toHex((*((char*)&m_Uuid[0]+ 2) >> 4) & 0b1111);
		uuid[1] = toHex(*((char*)&m_Uuid[0] + 3) & 0b1111);
		uuid[0] = toHex((*((char*)&m_Uuid[0] + 3) >> 4) & 0b1111);

		//time-mid
		uuid[12] = toHex(*((char*)&m_Uuid[0] + 4) & 0b1111);
		uuid[11] = toHex((*((char*)&m_Uuid[0] + 4) >> 4) & 0b1111);
		uuid[10] = toHex(*((char*)&m_Uuid[0] + 5) & 0b1111);
		uuid[9] = toHex((*((char*)&m_Uuid[0] + 5) >> 4) & 0b1111);

		//time-high-and-version
		uuid[17] = toHex(*((char*)&m_Uuid[0] +  6) & 0b1111);
		uuid[16] = toHex((*((char*)&m_Uuid[0] + 6) >> 4) & 0b1111);
		uuid[15] = toHex(*((char*)&m_Uuid[0] +  7) & 0b1111);
		uuid[14] = toHex((*((char*)&m_Uuid[0] + 7) >> 4) & 0b1111);

		//clock-seq-and-reserved
		uuid[20] = toHex(*((char*)&m_Uuid[1]) & 0b1111);
		uuid[19] = toHex((*((char*)&m_Uuid[1]) >> 4) & 0b1111);
		//clock-seq-low
		uuid[22] = toHex(*((char*)&m_Uuid[1] +  1) & 0b1111);
		uuid[21] = toHex((*((char*)&m_Uuid[1] + 1) >> 4) & 0b1111);

		//node
		uuid[25] = toHex(*((char*)&m_Uuid[1] +  2) & 0b1111);
		uuid[24] = toHex((*((char*)&m_Uuid[1] + 2) >> 4) & 0b1111);
		uuid[27] = toHex(*((char*)&m_Uuid[1] +  3) & 0b1111);
		uuid[26] = toHex((*((char*)&m_Uuid[1] + 3) >> 4) & 0b1111);
		uuid[29] = toHex(*((char*)&m_Uuid[1] +  4) & 0b1111);
		uuid[28] = toHex((*((char*)&m_Uuid[1] + 4) >> 4) & 0b1111);
		uuid[31] = toHex(*((char*)&m_Uuid[1] +  5) & 0b1111);
		uuid[30] = toHex((*((char*)&m_Uuid[1] + 5) >> 4) & 0b1111);
		uuid[33] = toHex(*((char*)&m_Uuid[1] +  6) & 0b1111);
		uuid[32] = toHex((*((char*)&m_Uuid[1] + 6) >> 4) & 0b1111);
		uuid[35] = toHex(*((char*)&m_Uuid[1] +  7) & 0b1111);
		uuid[34] = toHex((*((char*)&m_Uuid[1] + 7) >> 4) & 0b1111);
		return uuid;
	}

	bool UUID::operator==(const UUID& other) const
	{
		return (this->m_Uuid[0] == other.m_Uuid[0] && this->m_Uuid[1] == other.m_Uuid[1]);
	}
}

