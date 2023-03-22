#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class AsteroidScript : public ScriptableGameObject
	{
	public:
		enum Size { Small = 0, Medium, Large };
	public:
		/// <summary>
		/// AsteroidScript constructor
		/// </summary>
		/// <param name="angle">The angle in which the asteroid should start moving</param>
		AsteroidScript(float angle);
		/// <summary>
		/// AsteroidScript constructor
		/// </summary>
		/// <param name="angle">The angle in which the asteroid should start moving</param>
		/// <param name="size">The size of the asteroid (See the AsteroidScript::Size enum)</param>
		AsteroidScript(float angle, Size size);

		/// <summary>
		/// When the AsteroidScript is created
		/// </summary>
		void OnCreate() override;
		/// <summary>
		/// When the AsteroidScript is destroyed
		/// </summary>
		void OnDestroy() override;
		/// <summary>
		/// Update the position of the asteroid
		/// </summary>
		/// <param name="ts">The amount of time passed (in seconds)</param>
		void OnUpdate(Timestep ts) override;

		/// <summary>
		/// Destroy the asteroid Game Object. And award a score to the player based on the size 
		/// of the asteroid
		/// </summary>
		/// <param name="awardScore">If the player should be awarded a score</param>
		void Destroy(bool awardScore = true);

		/// <summary>
		/// Get the velocity of the asteroid
		/// </summary>
		/// <returns>The velocity of the asteroid</returns>
		glm::vec3 GetVelocity() const { return m_Velocity * m_Speed; }
	private:
		/// <summary>
		/// Creates two new asteroids of a smaller size
		/// </summary>
		void Split();
		/// <summary>
		/// Moves the asteroid back onto the screen on the opposite side (left/right top/bottom)
		/// </summary>
		void MoveOntoScreen();
	private:
		const float m_MinSpeed = 1.5f;
		const float m_SpeedIncAmount = 1.75f;
		const int m_MinSplitAngle = 4;
		const int m_MaxSplitAngle = 12;

		float m_Angle;
		glm::vec3 m_Velocity;
		Size m_Size;
		float m_Speed = m_MinSpeed;
	};
}