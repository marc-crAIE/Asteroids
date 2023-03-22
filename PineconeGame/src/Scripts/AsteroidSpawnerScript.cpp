#include "AsteroidSpawnerScript.h"

#include "AsteroidScript.h"
#include "GameLayer.h"

#include <random>

namespace AsteroidsGame
{
	static std::mt19937 s_Engine;
	static std::uniform_int_distribution<int> s_UniformDistribution(0, 360);

	void AsteroidSpawnerScript::OnCreate()
	{
		// TODO: Spawn an initial number of asteroids
	}

	void AsteroidSpawnerScript::OnDestroy()
	{

	}

	void AsteroidSpawnerScript::OnUpdate(Timestep ts)
	{
		// Get all of the asteroids
		auto asteroids = GetScene()->GetGameObjectsByTag("Asteroid");

		// Check to make sure we have not exceeded the max number of asteroids to spawn
		// Note: Does not matter if splitting asteroids passes this number. Really m_MaxAsteroids is more
		//		 like m_MinAsteroids
		if (asteroids.size() < m_MaxAsteroids)
		{
			// Increment the elapsed time since the last asteroid spawn
			m_LastAsteroidSpawn += ts;
			// If the last time an asteroid was spawned passed the time for another one to spawn
			if (m_LastAsteroidSpawn >= m_AsteroidSpawnTime)
			{
				// Spawn a new asteroid
				SpawnAsteroid();
			}
		}
	}

	void AsteroidSpawnerScript::Reset()
	{
		// Reset the values to default
		m_LastAsteroidSpawn = 0.0f;
	}

	void AsteroidSpawnerScript::SpawnAsteroid()
	{
		// Pick a random angle between 0 and 360
		float angle = s_UniformDistribution(s_Engine);

		// Create a new asteroid game object
		GameObject asteroid = GetScene()->CreateGameObject("Asteroid");
		// Add a NativeScriptComponent and bind AsteroidScript to it with angle passed as an argument
		asteroid.AddComponent<NativeScriptComponent>().Bind<AsteroidScript>(angle);

		// Get the primary camera component
		auto& camera = GetScene()->GetPrimaryCameraGameObject().GetComponent<CameraComponent>().Camera;

		// TODO: See the comment inside UILayer::OnUpdate to know how this can be improved
		float orthoSize = camera.GetOrthographicSize();
		float aspectRatio = camera.GetAspectRatio();

		float orthoLeft = -orthoSize * aspectRatio * 0.5f;
		float orthoRight = orthoSize * aspectRatio * 0.5f;
		float orthoBottom = -orthoSize * 0.5f;
		float orthoTop = orthoSize * 0.5f;

		// Get the transform of the asteroid
		auto& transform = asteroid.GetComponent<TransformComponent>();

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