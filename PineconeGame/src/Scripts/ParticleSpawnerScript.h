#pragma once

#include "Utils/ParticleSystem.h"

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class ParticleSpawnerScript : public ScriptableGameObject
	{
	public:
		ParticleSpawnerScript();

		void OnCreate() override;
		void OnDestroy() override;
		void OnUpdate(Timestep ts) override;

		void Emit(glm::vec3 postion, const int count = 1);
	private:
		ParticleSystem m_ParticleSystem;
		ParticleProps m_Particle;
	};
}