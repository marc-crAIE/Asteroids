#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class AsteroidScript : public ScriptableGameObject
	{
	public:
		enum Size { Small = 0, Medium, Large };
	public:
		AsteroidScript(float angle);
		AsteroidScript(float angle, Size size);

		void OnCreate() override;
		void OnDestroy() override;
		void OnUpdate(Timestep ts) override;

		void Destroy(bool awardScore = true);

		glm::vec3 GetVelocity() const { return m_Velocity * m_Speed; }
	private:
		void Split();
		void MoveOntoScreen();
	private:
		const float m_MinSpeed = 1.5f;
		const float m_SpeedIncAmount = 1.75f;
		const int m_MinSplitAngle = 4;
		const int m_MaxSplitAngle = 12;

		float m_Angle;
		glm::vec3 m_Velocity;
		Size m_Size;
		float m_Speed = m_MinSpeed;
	};
}