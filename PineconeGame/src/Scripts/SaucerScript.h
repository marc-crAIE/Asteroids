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
	private:
		void MoveOntoScreen();
	private:
		Type m_Type;
		glm::vec3 m_Velocity = { 0.0f, 0.0f, 0.0f };
		const float m_Speed = 3.0f;
	};
}