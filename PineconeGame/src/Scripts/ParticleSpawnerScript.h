#pragma once

#include "Utils/ParticleSystem.h"

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class ParticleSpawnerScript : public ScriptableGameObject
	{
	public:
		/// <summary>
		/// The ParticleSpawnerScript constructor
		/// </summary>
		ParticleSpawnerScript();

		/// <summary>
		/// When the ParticleSpawnerScript is created
		/// </summary>
		void OnCreate() override;
		/// <summary>
		/// When the ParticleSpawnerScript is destroyed
		/// </summary>
		void OnDestroy() override;
		/// <summary>
		/// Update the Particle System to update the positions of the particles and render them onto the screen
		/// </summary>
		/// <param name="ts">The amount of time passed (in seconds)</param>
		void OnUpdate(Timestep ts) override;

		/// <summary>
		/// Emit particles onto the screen
		/// </summary>
		/// <param name="postion">The position of where the particles will start</param>
		/// <param name="count">The amount of particles to emit</param>
		void Emit(glm::vec3 postion, const int count = 1);
	private:
		ParticleSystem m_ParticleSystem;
		ParticleProps m_Particle;
	};
}