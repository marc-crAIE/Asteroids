#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class SaucerScript : public ScriptableGameObject
	{
	public:
		enum Type { Small = 0, Large };
	public:
		SaucerScript(Type type);

		void OnCreate() override;
		void OnDestroy() override;
		void OnUpdate(Timestep ts) override;

		void Damage(bool awardScore = true);
	private:
		void Shoot();
		void Wander(Timestep ts);

		void MoveOntoScreen();
	private:
		Type m_Type;
		glm::vec3 m_Velocity = { 0.0f, 0.0f, 0.0f };
		const float m_Speed = 3.0f;
		const float m_SmallSaucerSize = 2.0f;
		const float m_LargeSaucerSize = 3.0f;

		float m_ShootTime = 0.0f;
		const float m_MaxShootTime = 2.5f;

		float m_ChangeDirectionElapsed = 0.0f;
		float m_ChangeDirectionTime = m_MaxChangeDirectionTime;
		const float m_MinChangeDirectionTime = 3.0f;
		const float m_MaxChangeDirectionTime = 8.0f;
	};
}