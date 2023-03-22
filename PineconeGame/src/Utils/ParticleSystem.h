#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	struct ParticleProps
	{
		glm::vec2 Position;
		glm::vec2 Velocity, VelocityVariation;
		glm::vec4 ColorBegin, ColorEnd;
		float SizeBegin, SizeEnd, SizeVariation;
		float LifeTime = 1.0f;
	};

	class ParticleSystem
	{
	public:
		/// <summary>
		/// ParticleSystem constructor
		/// </summary>
		/// <param name="maxParticles">The maximum number of particles to display</param>
		ParticleSystem(uint32_t maxParticles);

		/// <summary>
		/// Update the particles on screen and render them
		/// </summary>
		/// <param name="ts">The amount of time passed (in seconds)</param>
		void OnUpdate(Timestep ts);

		/// <summary>
		/// Emit a particle
		/// </summary>
		/// <param name="particleProps">The particle properties</param>
		void Emit(const ParticleProps& particleProps);
	private:
		struct Particle
		{
			glm::vec2 Position;
			glm::vec2 Velocity;
			glm::vec4 ColorBegin, ColorEnd;
			float Rotation = 0.0f;
			float SizeBegin, SizeEnd;

			float LifeTime = 1.0f;
			float LifeRemaining = 0.0f;

			bool Active = false;
		};
	private:
		std::vector<Particle> m_Particles;
		uint32_t m_PoolIndex = 0;
	};
}