#include "Physics2D.h"

namespace AsteroidsGame
{
	bool Physics2D::CheckRectCollision(GameObject& a, GameObject& b)
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
}