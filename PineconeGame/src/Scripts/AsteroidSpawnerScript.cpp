#include "AsteroidSpawnerScript.h"

#include "AsteroidScript.h"

#include <random>

namespace AsteroidsGame
{
	static std::mt19937 s_Engine;
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
		float angle = s_UniformDistribution(s_Engine);

		GameObject asteroid = GetScene()->CreateGameObject("Asteroid");
		asteroid.AddComponent<NativeScriptComponent>().Bind<AsteroidScript>(angle);

		auto& transform = asteroid.GetComponent<TransformComponent>();
		auto& camera = GetScene()->GetPrimaryCameraGameObject().GetComponent<CameraComponent>().Camera;

		float orthoSize = camera.GetOrthographicSize();
		float aspectRatio = camera.GetAspectRatio();

		float orthoLeft = -orthoSize * aspectRatio * 0.5f;
		float orthoRight = orthoSize * aspectRatio * 0.5f;
		float orthoBottom = -orthoSize * 0.5f;
		float orthoTop = orthoSize * 0.5f;

		// Right side of the screen
		if (angle > 315 || angle <= 45)
		{
			transform.Translation.x = orthoRight;
		}

		// Bottom of the screen
		else if (angle > 45 && angle <= 135)
		{
			transform.Translation.y = orthoBottom;
		}

		// Left side of the screen
		else if (angle > 135 && angle <= 225)
		{
			transform.Translation.x = orthoLeft;
		}

		// Top of the screen
		else
		{
			transform.Translation.y = orthoTop;
		}

		m_LastAsteroidSpawn = 0.0f;
	}
}