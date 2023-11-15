#pragma once
#include <glm/glm.hpp>

namespace Orchis
{
	
	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(glm::vec3 position, float fieldOfView, float speed);
		~PerspectiveCamera() = default;

		void Update();

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetProjMatrix() const { return m_ProjMatrix; }
		const glm::mat4& GetTransform() const { return m_ViewProj; }
		const glm::vec3& GetPosition() const { return m_Position; }
		
		void SetProjMatrix(float width, float height);

		glm::vec3 m_Position;
		glm::vec3 m_CameraFront;
	private:
		float m_FieldOfView;
		float m_CameraSpeed;
		float m_Yaw, m_Pitch, m_Sensitivity;
		glm::vec3 m_CameraUp;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjMatrix;
		glm::mat4 m_ViewProj;
	};
}