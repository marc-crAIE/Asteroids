#include "Physics2D.h"

namespace AsteroidsGame
{
	bool Physics2D::CheckRectCollision(GameObject a, GameObject b)
	{
		if (a.HasComponent<TransformComponent>() && b.HasComponent<TransformComponent>())
		{
			auto& aTransform = a.GetComponent<TransformComponent>();
			auto& bTransform = b.GetComponent<TransformComponent>();

			bool collisionX = aTransform.Translation.x < bTransform.Translation.x + bTransform.Scale.x &&
				aTransform.Translation.x + aTransform.Scale.x > bTransform.Translation.x;
			bool collisionY = aTransform.Translation.y < bTransform.Translation.y + bTransform.Scale.y &&
				aTransform.Translation.y + aTransform.Scale.y > bTransform.Translation.y;

			return collisionX && collisionY;
		}
		return false;
	}

	bool Physics2D::CheckRectCollision(GameObject a, GameObject b, glm::vec3 aVelocity, glm::vec3 bVelocity)
	{
		auto& aTransform = a.GetComponent<TransformComponent>();
		auto& bTransform = b.GetComponent<TransformComponent>();

		bool collisionX = aTransform.Translation.x + aVelocity.x < bTransform.Translation.x + bTransform.Scale.x + bVelocity.x &&
			aTransform.Translation.x + aTransform.Scale.x + aVelocity.x > bTransform.Translation.x + bVelocity.x;
		bool collisionY = aTransform.Translation.y + aVelocity.y < bTransform.Translation.y + bTransform.Scale.y + bVelocity.y &&
			aTransform.Translation.y + aTransform.Scale.y + aVelocity.y > bTransform.Translation.y + bVelocity.y;

		return collisionX && collisionY;
	}
	bool Physics2D::CheckCircleCollision(GameObject a, GameObject b)
	{
		auto& aTransform = a.GetComponent<TransformComponent>();
		auto& bTransform = b.GetComponent<TransformComponent>();

		float y = bTransform.Translation.y - aTransform.Translation.y;
		float x = bTransform.Translation.x - aTransform.Translation.x;
		float distance = x * x + y * y;

		float aRadius = glm::dot(aTransform.Scale.x, aTransform.Scale.y);
		float bRadius = glm::dot(bTransform.Scale.x, bTransform.Scale.y);

		if (distance <= aRadius + bRadius)
		{
			return true;
		}
		return false;
	}
}