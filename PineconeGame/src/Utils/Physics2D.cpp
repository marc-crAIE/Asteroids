#include "Physics2D.h"

namespace AsteroidsGame
{
	bool Physics2D::CheckRectCollision(GameObject a, GameObject b)
	{
		// Check if A and B have a transform
		if (a.HasComponent<TransformComponent>() && b.HasComponent<TransformComponent>())
		{
			// Grab the transforms of the A and B Game Objects
			auto& aTransform = a.GetComponent<TransformComponent>();
			auto& bTransform = b.GetComponent<TransformComponent>();

			// Test for a collision on the x axis
			bool collisionX = aTransform.Translation.x < bTransform.Translation.x + bTransform.Scale.x &&
				aTransform.Translation.x + aTransform.Scale.x > bTransform.Translation.x;
			// Test for a collision on the y axis
			bool collisionY = aTransform.Translation.y < bTransform.Translation.y + bTransform.Scale.y &&
				aTransform.Translation.y + aTransform.Scale.y > bTransform.Translation.y;

			// If both the x and y axes are colliding, then we have a collision
			return collisionX && collisionY;
		}
		// Return false if either or both objects don't have a transform
		return false;
	}

	bool Physics2D::CheckRectCollision(GameObject a, GameObject b, glm::vec3 aVelocity, glm::vec3 bVelocity)
	{
		// Check if A and B have a transform
		if (a.HasComponent<TransformComponent>() && b.HasComponent<TransformComponent>())
		{
			// Grab the transforms of the A and B Game Objects
			auto& aTransform = a.GetComponent<TransformComponent>();
			auto& bTransform = b.GetComponent<TransformComponent>();

			// Test for a collision on the x axis and factor in the A and B velocities
			bool collisionX = aTransform.Translation.x + aVelocity.x < bTransform.Translation.x + bTransform.Scale.x + bVelocity.x &&
				aTransform.Translation.x + aTransform.Scale.x + aVelocity.x > bTransform.Translation.x + bVelocity.x;
			// Test for a collision on the y axis and factor in the A and B velocities
			bool collisionY = aTransform.Translation.y + aVelocity.y < bTransform.Translation.y + bTransform.Scale.y + bVelocity.y &&
				aTransform.Translation.y + aTransform.Scale.y + aVelocity.y > bTransform.Translation.y + bVelocity.y;

			// If both the x and y axes are colliding, then we have a collision
			return collisionX && collisionY;
		}
		// Return false if either or both objects don't have a transform
		return false;
	}

	bool Physics2D::CheckCircleCollision(GameObject a, GameObject b)
	{
		// Check if A and B have a transform
		if (a.HasComponent<TransformComponent>() && b.HasComponent<TransformComponent>())
		{
			// Grab the transforms of the A and B Game Objects
			auto& aTransform = a.GetComponent<TransformComponent>();
			auto& bTransform = b.GetComponent<TransformComponent>();

			// Get the distance between the center of both game objects
			float y = bTransform.Translation.y - aTransform.Translation.y;
			float x = bTransform.Translation.x - aTransform.Translation.x;
			float distance = x * x + y * y;

			// Get the radius of both game objects by getting the dot product of the x and y values for their scale
			float aRadius = glm::dot(aTransform.Scale.x, aTransform.Scale.y);
			float bRadius = glm::dot(bTransform.Scale.x, bTransform.Scale.y);

			// Check if the distance between the objects is less or equal to their radii combined
			if (distance <= aRadius + bRadius)
			{
				// They are colliding
				return true;
			}
		}
		// Return false if they are not colliding or at least one object does not have a transform
		return false;
	}
}