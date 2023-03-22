#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class AsteroidSpawnerScript : public ScriptableGameObject
	{
	public:
		/// <summary>
		/// When the AsteroidSpawnerScript is created
		/// </summary>
		void OnCreate() override;
		/// <summary>
		/// When the AsteroidSpawnerScript is destroyed
		/// </summary>
		void OnDestroy() override;
		/// <summary>
		/// Spawn in an asteroid if enough time has passed unless the max number of asteroids is met
		/// </summary>
		/// <param name="ts">The amount of time passed (in seconds)</param>
		void OnUpdate(Timestep ts) override;

		/// <summary>
		/// Reset values to default
		/// </summary>
		void Reset();
	private:
		/// <summary>
		/// Spawn in a new asteroid on a random edge of the screen
		/// </summary>
		void SpawnAsteroid();
	private:
		const float m_AsteroidSpawnTime = 2.5f;
		float m_LastAsteroidSpawn = 0.0f;

		const float m_MaxAsteroids = 10.0f;
	};
}