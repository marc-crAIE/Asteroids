#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class AsteroidScript : public ScriptableGameObject
	{
	public:
		AsteroidScript(float angle);

		void OnCreate() override;
		void OnDestroy() override;
		void OnUpdate(Timestep ts) override;
	private:
		float m_Angle;
		glm::vec3 m_Velocity;

		const float m_Speed = 5.0f;
	};
}