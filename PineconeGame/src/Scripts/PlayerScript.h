#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class PlayerScript : public ScriptableGameObject
	{
	public:
		void OnCreate() override;
		void OnDestroy() override;
		void OnUpdate(Timestep ts) override;

		void AddLife();
		void RemoveLife();

		int GetLives() const { return m_Lives; }
	private:
		void ShootBullet();
		void CheckAsteroidCollision();
		void Hyperspace();

		void MovePlayerOntoScreen();
	private:
		Ref<Texture2D> m_NormalTexture;
		Ref<Texture2D> m_MoveTexture;

		glm::vec3 m_Velocity = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
		int m_Lives = 5;

		bool m_Invulnerable = false;
		float m_InvulnerabilityTime = 0.0f;
		const float m_MaxInvulnerability = 2.5f;

		const float m_HypespaceCooldown = 10.0f;
		float m_HyperspaceCooldownTime = m_HypespaceCooldown;

		const float m_Speed = 0.2f;
		const float m_RotationSpeed = 220.0f;
		const float m_SlowDownSpeed = 0.994f;

		const int m_MaxShots = 4;
		const float m_ShootCooldown = 0.4f;
		float m_LastShot = m_ShootCooldown;
	};
}