#include "ParticleSpawnerScript.h"

#include "GameLayer.h"

namespace AsteroidsGame
{
	ParticleSpawnerScript::ParticleSpawnerScript()
		: m_ParticleSystem(1000)
	{
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
		auto camera = GameLayer::Get().GetScene()->GetPrimaryCameraGameObject().GetComponent<CameraComponent>().Camera;

		Renderer2D::BeginScene(camera);
		m_ParticleSystem.OnUpdate(ts);
		Renderer2D::EndScene();
	}

	void ParticleSpawnerScript::Emit(glm::vec3 postion, const int count)
	{
		m_Particle.Position = { postion.x, postion.y };

		for (int i = 0; i < count; i++)
			m_ParticleSystem.Emit(m_Particle);
	}
}