#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class AsteroidSpawnerScript : public ScriptableGameObject
	{
	public:
		void OnCreate() override;
		void OnDestroy() override;
		void OnUpdate(Timestep ts) override;
	private:
		void SpawnAsteroid();
	private:
		float m_AsteroidSpawnTime = 2.5f;
		float m_LastAsteroidSpawn = 0.0f;

		const float m_MaxAsteroids = 10.0f;
	};
}