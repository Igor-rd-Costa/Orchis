#pragma once
#include "thread"
#include "PerspectiveCamera.h"
#include "Events.h"

struct EditorCameraData
{
	glm::vec3 position;
	glm::vec3 lookAt;
	float speed;
	float fieldOfView;
	float yaw;
	float pitch;
};

enum EditorEventType
{
	MOUSE_MOVE, CAMERA_MOVE
};

struct EditorMouseMoveEventArgs
{

};

struct EditorCameraMoveEventArgs
{
	EditorCameraData data;
};

class Editor
{
public:
	Editor(const EditorCameraData& data);
	~Editor();

	

	void static Run();
	void static Update();

	EditorCameraData static GetCameraData();
	static inline Orchis::PerspectiveCamera* GetCamera() { return s_EditorCamera; }
	void static SetEditorCameraData(EditorCameraData data);

	void static RegisterMouseMoveCallback(void (*callback)(EditorMouseMoveEventArgs e));
	void static RegisterCameraMoveCallback(void (*callback)(EditorCameraMoveEventArgs e));
	
private:
	static Orchis::PerspectiveCamera* s_EditorCamera;
	static std::thread s_EngineThread;
	static void(*s_MouseMoveCallback)(EditorMouseMoveEventArgs e);
	static void(*s_CameraMoveCallback)(EditorCameraMoveEventArgs e);
	static bool s_Running;
	void static OnMouseMove(Orchis::MouseMoveEvent* event);
	void static OnMouseButtonDown(Orchis::MouseButtonDownEvent* event);
	void static OnMouseButtonUp(Orchis::MouseButtonUpEvent* event);
};