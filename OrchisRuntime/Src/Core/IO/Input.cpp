#include "Input.h"
#include "Core/IO/Logger.h"
namespace Orchis {

	
	void Input::RegisterKeyPress(Key key)
	{
		int index = key / 8;
		int offset = key % 8;
		m_Buffer[index] |= (0b0011 << (offset * 4));
	}

	void Input::RegisterKeyRelease(Key key)
	{
		int index = key / 8;
		int offset = key % 8;
		m_Buffer[index] ^= (m_Buffer[index] & (0b0011 << (offset * 4)));
		m_Buffer[index] |= (0b0100 << (offset * 4));
	}

	bool Input::mWasKeyPressed(Key key) const
	{
		int index = key / 8;
		int offset = key % 8;
		return ((m_Buffer[index] >> (offset * 4)) & 0b0001) == 0b0001;
	}

	bool Input::mIsKeyDown(Key key) const
	{
		int index = key / 8;
		int offset = key % 8;
		return ((m_Buffer[index] >> (offset * 4)) & 0b0010) == 0b0010;
	}
	bool Input::mWasKeyReleased(Key key) const
	{
		int index = key / 8;
		int offset = key % 8;
		return ((m_Buffer[index] >> (offset * 4)) & 0b0100) == 0b0100;
	}

	void Input::ClearKeyPressState()
	{
		static int clearValue = 0b01010101010101010101010101010101;
		for (int i = 0; i < 47; i++) {
			m_Buffer[i] ^= (m_Buffer[i] & clearValue);
		}
	}

	void Input::SetStandartInput(const Input* input)
	{
		s_ActiveInput = input;
	}


	bool Input::WasKeyPressed(Key key)
	{
		return s_ActiveInput->mWasKeyPressed(key);
	}

	bool Input::WasKeyReleased(Key key)
	{
		return s_ActiveInput->mWasKeyReleased(key);
	}

	bool Input::IsKeyDown(Key key)
	{
		return s_ActiveInput->mIsKeyDown(key);
	}
}