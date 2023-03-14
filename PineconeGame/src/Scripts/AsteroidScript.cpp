#include "AsteroidScript.h"

namespace AsteroidsGame
{
	AsteroidScript::AsteroidScript(float angle)
		: m_Angle(angle)
	{
		m_Velocity = glm::vec3(glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)), 0.0f);
	}

	void AsteroidScript::OnCreate()
	{
	}

	void AsteroidScript::OnDestroy()
	{
	}

	void AsteroidScript::OnUpdate(Timestep ts)
	{
		auto& transform = GetComponent<TransformComponent>();
		transform.Translation += m_Velocity * m_Speed * (float)ts;
	}
}