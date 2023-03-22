#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class Physics2D
	{
	public:
		/// <summary>
		/// Check collision for two rectangular Game Objects A and B
		/// </summary>
		/// <param name="a">A Game Object to check collision for</param>
		/// <param name="b">The other Game Object A might be collising with</param>
		/// <returns>True if the two Game Objects are colliding</returns>
		static bool CheckRectCollision(GameObject a, GameObject b);

		/// <summary>
		/// Check collision for two rectangular Game Objects A and B and factor in their velocities
		/// </summary>
		/// <param name="a">A Game Object to check collision for</param>
		/// <param name="b">The other Game Object A might be collising with</param>
		/// <param name="aVelocity">The velocity of Game Object A</param>
		/// <param name="bVelocity">The velocity of Game Object B</param>
		/// <returns>True if the two Game Objects are colliding</returns>
		static bool CheckRectCollision(GameObject a, GameObject b, glm::vec3 aVelocity, glm::vec3 bVelocity);

		/// <summary>
		/// Check collision for two circular Game Objects A and B
		/// </summary>
		/// <param name="a">A Game Object to check collision for</param>
		/// <param name="b">The other Game Object A might be collising with</param>
		/// <returns>True if the two Game Objects are colliding</returns>
		static bool CheckCircleCollision(GameObject a, GameObject b);
	};
}