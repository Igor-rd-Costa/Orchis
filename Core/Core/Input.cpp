#include "OrchisPCH.h"
#include "Input.h"
#include "Application.h"

namespace Orchis {

	std::array<char, 96> Input::s_InputBuffer;
	bool Input::s_IsCursorVisible = true;
	bool Input::s_IsCursorClipped = false;

	#define BITS_PER_KEY 4
	#define KEYDOWN_VALUE 1
	#define KEYUP_VALUE 2
	#define KEYHOLD_VALUE 4
	#define KEYS_PER_CHAR 2
	#define ERASE_KEY_STATE_VALUE ((KEYDOWN_VALUE | KEYUP_VALUE) | ((KEYDOWN_VALUE | KEYUP_VALUE) << 4))

	bool Input::KeyPressed(Key key)
	{
		int index = (int)key / KEYS_PER_CHAR;
		int offset = (int)key % KEYS_PER_CHAR;
		if ((s_InputBuffer[index] & KEYDOWN_VALUE << (offset * BITS_PER_KEY)))
			return true;
		else
			return false;
	}

	bool Input::KeyReleased(Key key)
	{
		int index = (int)key / KEYS_PER_CHAR;
		int offset = (int)key % KEYS_PER_CHAR;
		if ((s_InputBuffer[index] & KEYUP_VALUE << (offset * BITS_PER_KEY)))
			return true;
		else
			return false;
	}

	bool Input::IsKeyDown(Key key)
	{
		int index = (int)key / KEYS_PER_CHAR;
		int offset = (int)key % KEYS_PER_CHAR;
		if ((s_InputBuffer[index] & KEYHOLD_VALUE << (offset * BITS_PER_KEY)))
			return true;
		else
			return false;
	}

	void Input::RegisterKeyDown(Key key)
	{
		int index = (int)key / KEYS_PER_CHAR;
		int offset = (int)key % KEYS_PER_CHAR;
		s_InputBuffer[index] |= (KEYDOWN_VALUE | KEYHOLD_VALUE) << (offset * BITS_PER_KEY);
	}

	void Input::RegisterKeyUp(Key key)
	{
		int index = (int)key / KEYS_PER_CHAR;
		int offset = (int)key % KEYS_PER_CHAR;
		s_InputBuffer[index] |= KEYUP_VALUE << (offset * BITS_PER_KEY);
		s_InputBuffer[index] &= s_InputBuffer[index] ^ (KEYDOWN_VALUE | KEYHOLD_VALUE) << (offset * BITS_PER_KEY);
	}

	void Input::UpdateState()
	{
		if (s_IsCursorClipped) Input::CenterCursor();
		for (char& keyState : s_InputBuffer)
		{
			keyState &= ~ERASE_KEY_STATE_VALUE;
		}
	}
}