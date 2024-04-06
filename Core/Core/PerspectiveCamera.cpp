#include "PerspectiveCamera.h"
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>
#include "Application.h"
namespace Orchis {

	PerspectiveCamera::PerspectiveCamera(glm::vec3 position, float fieldOfView, float speed)
		: m_Position(position), m_CameraFront({ 0.0f, 1.0f, 0.0f }), m_FieldOfView(fieldOfView), 
		m_CameraSpeed(speed), m_CameraUp({ 0.0f, 0.0f, 1.0f }), m_Yaw(0.0f), m_Pitch(0.0f), m_Sensitivity(0.3f), 
		m_ViewMatrix(glm::mat4(1.0f)), m_ViewProj(m_ProjMatrix * m_ViewMatrix)
	{
		Window* window = Application::GetWindow();
		float width = 1.0f, height = 1.0f;
		if (window)
		{
			width = static_cast<float>(window->GetWidth());
			height = static_cast<float>(window->GetHeight());
		}
		m_ProjMatrix = glm::perspective(glm::radians(fieldOfView), width / height, 0.1f, 100.0f);
		m_ProjMatrix[1][1] *= -1;

		glm::mat4 test = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}

	void PerspectiveCamera::Update()
	{
		static CursorPos cursorPosition = Input::GetCursorPos();
		/*cursorPosition = Input::GetCursorPos();
		std::pair<long, long> center = Application::GetWindow()->GetCenter();
		m_Yaw += (cursorPosition.x - center.first) * m_Sensitivity;
		m_Pitch += (center.second - cursorPosition.y) * m_Sensitivity;

		if (m_Pitch > 89.0f) m_Pitch = 89.0f;
		if (m_Pitch < -89.0f) m_Pitch = -89.0f;

		glm::vec3 direction{};
		direction.y = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
		direction.z = glm::sin(glm::radians(m_Pitch));
		direction.x = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
		m_CameraFront = glm::normalize(direction);

		if (Input::IsKeyDown(Key::KEY_W))
		{
			m_Position += m_CameraSpeed * m_CameraFront * Time::GetDelta();
		}
		else if (Input::IsKeyDown(Key::KEY_S))
		{
			m_Position -= m_CameraSpeed * m_CameraFront * Time::GetDelta();
		}

		if (Input::IsKeyDown(Key::KEY_D))
		{
			m_Position += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CameraSpeed * Time::GetDelta();
		}
		else if (Input::IsKeyDown(Key::KEY_A))
		{
			m_Position -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CameraSpeed * Time::GetDelta();
		}

		if (Input::IsKeyDown(Key::KEY_SPACE))
		{
			m_Position += m_CameraUp * m_CameraSpeed * Time::GetDelta();
		}
		else if (Input::IsKeyDown(Key::KEY_LSHIFT))
		{
			m_Position -= m_CameraUp * m_CameraSpeed * Time::GetDelta();
		}*/

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_CameraFront, m_CameraUp);
		m_ViewProj = m_ProjMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetProjMatrix(float width, float height)
	{
		m_ProjMatrix = glm::perspective(m_FieldOfView, width / height, 0.1f, 100.0f);
		m_ProjMatrix[1][1] *= -1;
	}

}
