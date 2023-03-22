#include "ParticleSystem.h"

#include <random>

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

namespace AsteroidsGame
{
	class Random
	{
	public:
		static void Init()
		{
			// Initialize the randomizer
			s_RandomEngine.seed(std::random_device()());
		}

		/// <summary>
		/// Get a random float value between 0.0 and 1.0
		/// </summary>
		/// <returns>A random float</returns>
		static float Float()
		{
			// This is done by generating a random integer number and dividing it by the max value for a uint32
			return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
		}

	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};

	std::mt19937 Random::s_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

	ParticleSystem::ParticleSystem(uint32_t maxParticles)
		: m_PoolIndex(maxParticles - 1)
	{
		// Initialize the particle pool based on a set size
		m_Particles.resize(maxParticles);
	}

	void ParticleSystem::OnUpdate(Timestep ts)
	{
		for (auto& particle : m_Particles)
		{
			// If the particle is not active, go to the next one
			// TODO: Maybe sort particles by active and non-active by putting the non-active
			//		 ones last so we can just exit for for-loop when we hit the first one
			if (!particle.Active)
				continue;

			// If the particle has ran out of its lifetime, set it as non-active
			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				continue;
			}

			// Update the particles lifetime, position, and rotation
			particle.LifeRemaining -= ts;
			particle.Position += particle.Velocity * (float)ts;
			particle.Rotation += 0.01f * ts;

			// Linear interpolate between the start color and end color of the particle based on its lifetime
			float life = particle.LifeRemaining / particle.LifeTime;
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);

			// Do the same for its size
			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

			// Get the position and draw it to the screen as a rotated quad
			glm::vec3 position = { particle.Position.x, particle.Position.y, 0.2f };
			Renderer2D::DrawRotatedQuad(position, { size, size }, particle.Rotation, color);
		}
	}

	void ParticleSystem::Emit(const ParticleProps& particleProps)
	{
		// Get a reference to a particle at the current index
		Particle& particle = m_Particles[m_PoolIndex];

		// Set all of its properties to be a copy of the specified particle properties

		particle.Active = true;
		particle.Position = particleProps.Position;
		particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

		// Velocity
		particle.Velocity = particleProps.Velocity;
		particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
		particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

		// Color
		particle.ColorBegin = particleProps.ColorBegin;
		particle.ColorEnd = particleProps.ColorEnd;

		particle.LifeTime = particleProps.LifeTime;
		particle.LifeRemaining = particleProps.LifeTime;
		particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
		particle.SizeEnd = particleProps.SizeEnd;

		// Decrement the current index within the size of the particle pool
		m_PoolIndex = --m_PoolIndex % m_Particles.size();
	}
}