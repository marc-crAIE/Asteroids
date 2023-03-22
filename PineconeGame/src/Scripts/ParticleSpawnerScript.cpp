#include "ParticleSpawnerScript.h"

#include "GameLayer.h"

namespace AsteroidsGame
{
	ParticleSpawnerScript::ParticleSpawnerScript()
		: m_ParticleSystem(1000)
	{
		// Setup our particle properties
		m_Particle.ColorBegin = { 0.9f, 0.9f, 0.9f, 1.0f };
		m_Particle.ColorEnd = { 0.3f, 0.3f, 0.3f, 1.0f };
		m_Particle.SizeBegin = 0.3f, m_Particle.SizeVariation = 0.15f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 1.0f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 3.0f, 3.0f };
		m_Particle.Position = { 0.0f, 0.0f };
	}

	void ParticleSpawnerScript::OnCreate()
	{
	}

	void ParticleSpawnerScript::OnDestroy()
	{
	}

	void ParticleSpawnerScript::OnUpdate(Timestep ts)
	{
		// Get the primary camera component
		auto camera = GameLayer::Get().GetScene()->GetPrimaryCameraGameObject().GetComponent<CameraComponent>().Camera;

		// Begin the 2D renderer as we want to draw some stuff in 2D
		// Note: This kinda sucks at the moment as everytime Renderer2D::BeginScene is called it does at least 1 new draw call.
		//		 A TODO would be to have UI components and create a UI system in the engine itself so they can be drawn inside of the scene
		Renderer2D::BeginScene(camera);
		// Update the particle system
		m_ParticleSystem.OnUpdate(ts);
		// End the 2D renderer
		Renderer2D::EndScene();
	}

	void ParticleSpawnerScript::Emit(glm::vec3 postion, const int count)
	{
		// Set the particle position
		m_Particle.Position = { postion.x, postion.y };

		// Emit the number of particles specified
		for (int i = 0; i < count; i++)
			m_ParticleSystem.Emit(m_Particle);
	}
}