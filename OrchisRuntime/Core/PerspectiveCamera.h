#pragma once
#include "Macros.h"
#include "Window.h"
#include "Application.h"
#include <glm/glm.hpp>

namespace Orchis
{
	
	class OC_API PerspectiveCamera
	{
	public:
		PerspectiveCamera(const glm::vec3& position, float fieldOfView, float speed, const glm::vec3& lookAt = { 0.0f, 1.0f, 0.0f }, float yaw = 0.0F, float pitch = 0.0F);
		~PerspectiveCamera() = default;

		void Update();
		void Move();

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetProjMatrix() const { return m_ProjMatrix; }
		const glm::mat4& GetTransform() const { return m_ViewProj; }

		void SetProjMatrix(float width, float height);

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetFront() const { return m_CameraFront; }
		float GetFieldOfView() const { return m_FieldOfView; }
		float GetSpeed() const { return m_CameraSpeed; }
		float GetYaw() const { return m_Yaw; }
		float GetPitch() const { return m_Pitch; }

		void SetPosition(const glm::vec3& position) { m_Position = position; }
		void SetFront(const glm::vec3& front) { m_CameraFront = front; }
		void SetFieldOfView(float fov) 
		{ 

			m_FieldOfView = fov;
			std::cout << "FOV: " << fov << "\n";
			Window* window = Application::GetWindow();
			SetProjMatrix(window->GetWidth(), window->GetHeight());
		}
		void SetSpeed(float speed) { m_CameraSpeed = speed; }
		void SetYaw(float yaw) { m_Yaw = yaw; }
		void SetPitch(float pitch) { m_Pitch = pitch; }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_CameraFront;
		float m_FieldOfView;
		float m_CameraSpeed;
		float m_Yaw, m_Pitch, m_Sensitivity;
		glm::vec3 m_CameraUp;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjMatrix;
		glm::mat4 m_ViewProj;
	};
}