#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class BulletScript : public ScriptableGameObject
	{
	public:
		BulletScript(float angle, bool playerOwned = true);

		void OnCreate() override;
		void OnDestroy() override;
		void OnUpdate(Timestep ts) override;
	private:
		void CheckAsteroidCollision();
		void CheckSaucerCollision();
		void CheckPlayerCollision();

		void MoveOntoScreen();

		glm::vec3 GetVelocity() const { return m_Velocity * m_Speed; }
	private:
		float m_Angle;
		glm::vec3 m_Velocity;
		float m_Lifetime = 0.0f;
		bool m_PlayerOwned;

		const glm::vec3 m_Size = { 0.1f, 0.1f, 0.1f };

		const float m_Speed = 25.0f;
		const float m_MaxLifetime = 1.25f;
	};
}