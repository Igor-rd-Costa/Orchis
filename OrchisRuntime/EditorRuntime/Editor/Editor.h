#pragma once
#include "thread"
#include "PerspectiveCamera.h"
#include "Events.h"

enum EditorEventType
{
	MOUSE_MOVE
};

struct EditorEventArgs
{
	EditorEventType type;
};

class Editor
{
public:
	Editor();
	~Editor();

	void static Run();
	void static Update();

	void static RegisterEventCallback(void (*callback)(EditorEventArgs e));
private:
	static Orchis::PerspectiveCamera* s_EditorCamera;
	static std::thread s_EngineThread;
	static std::vector<void(*)(EditorEventArgs e)> s_Callbacks;
	static bool s_Running;
	void static OnMouseMove(Orchis::MouseMoveEvent* event);
	void static OnMouseButtonDown(Orchis::MouseButtonDownEvent* event);
	void static OnMouseButtonUp(Orchis::MouseButtonUpEvent* event);
};