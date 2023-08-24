#include "OrchisPCH.h"
#include "Input.h"
#include <iostream>
#include <bitset>
namespace Orchis {

	char Input::m_InputBuffer[96];

	#define BITS_PER_KEY 4
	#define KEYDOWN_VALUE 1
	#define KEYUP_VALUE 2
	#define KEYHOLD_VALUE 4
	#define KEYS_PER_CHAR 2
	#define ERASE_KEY_STATE_VALUE 51

	bool Input::KeyPressed(Key key)
	{
		int index = (int)key / KEYS_PER_CHAR;
		int offset = (int)key % KEYS_PER_CHAR;
		if ((m_InputBuffer[index] & KEYDOWN_VALUE << (offset * BITS_PER_KEY)))
			return true;
		else
			return false;
	}

	bool Input::KeyReleased(Key key)
	{
		int index = (int)key / KEYS_PER_CHAR;
		int offset = (int)key % KEYS_PER_CHAR;
		if ((m_InputBuffer[index] & KEYUP_VALUE << (offset * BITS_PER_KEY)))
			return true;
		else
			return false;
	}

	bool Input::IsKeyDown(Key key)
	{
		int index = (int)key / KEYS_PER_CHAR;
		int offset = (int)key % KEYS_PER_CHAR;
		if ((m_InputBuffer[index] & KEYHOLD_VALUE << (offset * BITS_PER_KEY)))
			return true;
		else
			return false;
	}

	void Input::RegisterKeyDown(Key key)
	{
		int index = (int)key / KEYS_PER_CHAR;
		int offset = (int)key % KEYS_PER_CHAR;
		m_InputBuffer[index] |= (KEYDOWN_VALUE | KEYHOLD_VALUE) << (offset * BITS_PER_KEY);
	}

	void Input::RegisterKeyUp(Key key)
	{
		int index = (int)key / KEYS_PER_CHAR;
		int offset = (int)key % KEYS_PER_CHAR;
		m_InputBuffer[index] |= KEYUP_VALUE << (offset * BITS_PER_KEY);
		m_InputBuffer[index] &= KEYUP_VALUE << (offset * BITS_PER_KEY);
	}

	void Input::UpdateState()
	{
		for (int index = 0; index < 96; index++)
		{
				m_InputBuffer[index] &= ~ERASE_KEY_STATE_VALUE;
		}
	}


}