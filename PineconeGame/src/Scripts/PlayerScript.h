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
	private:
		void ShootBullet();
		void MovePlayerOntoScreen();
	private:
		Ref<Texture2D> m_NormalTexture;
		Ref<Texture2D> m_MoveTexture;

		glm::vec3 m_Velocity = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;

		const float m_Speed = 0.06f;
		const float m_RotationSpeed = 200.0f;
		const float m_ShootCooldown = 0.5f;
		float m_LastShot = m_ShootCooldown;
	};
}