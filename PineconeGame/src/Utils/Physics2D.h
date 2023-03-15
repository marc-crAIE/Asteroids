#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class Physics2D
	{
	public:
		static bool CheckRectCollision(GameObject a, GameObject b);
		static bool CheckRectCollision(GameObject a, GameObject b, glm::vec3 aVelocity, glm::vec3 bVelocity);

		static bool CheckCircleCollision(GameObject a, GameObject b);
	};
}