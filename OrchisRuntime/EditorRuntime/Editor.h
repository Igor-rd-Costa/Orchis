#pragma once
#include "thread"

class Editor
{
public:
	Editor();
	~Editor();

	void Run();

private:
	std::thread m_EngineThread;
	bool m_Running = true;
};