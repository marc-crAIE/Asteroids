#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class BulletScript : public ScriptableGameObject
	{
	public:
		BulletScript(float angle);

		void OnCreate() override;
		void OnDestroy() override;
		void OnUpdate(Timestep ts) override;
	private:
		void CheckAsteroidCollision(Timestep ts);
		void MoveOntoScreen();

		glm::vec3 GetVelocity() const { return m_Velocity * m_Speed; }
	private:
		float m_Angle;
		glm::vec3 m_Velocity;
		float m_Lifetime = 0.0f;

		const glm::vec3 m_Size = { 0.1f, 0.1f, 0.1f };

		const float m_Speed = 18.0f;
		const float m_MaxLifetime = 2.0f;
	};
}