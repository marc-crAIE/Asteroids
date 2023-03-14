#include "AsteroidSpawnerScript.h"

#include "AsteroidScript.h"

#include <random>

namespace AsteroidsGame
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<int> s_UniformDistribution(0, 360);

	void AsteroidSpawnerScript::OnCreate()
	{

	}

	void AsteroidSpawnerScript::OnDestroy()
	{

	}

	void AsteroidSpawnerScript::OnUpdate(Timestep ts)
	{
		auto asteroids = GetScene()->GetGameObjectsByTag("Asteroid");

		if (asteroids.size() < m_MaxAsteroids)
		{
			m_LastAsteroidSpawn += ts;
			if (m_LastAsteroidSpawn >= m_AsteroidSpawnTime)
			{
				SpawnAsteroid();
			}
		}
	}

	void AsteroidSpawnerScript::SpawnAsteroid()
	{
		GameObject asteroid = GetScene()->CreateGameObject("Asteroid");
		asteroid.AddComponent<SpriteComponent>();
		asteroid.AddComponent<NativeScriptComponent>().Bind<AsteroidScript>(s_UniformDistribution(s_Engine));
		m_LastAsteroidSpawn = 0.0f;
	}
}