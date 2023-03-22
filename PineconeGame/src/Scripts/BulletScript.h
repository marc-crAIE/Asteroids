#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class BulletScript : public ScriptableGameObject
	{
	public:
		/// <summary>
		/// The BulletScript constructor. Change player owned to ensure the player is damaged by the 
		/// correct bullets from the right object
		/// </summary>
		/// <param name="angle">The angle the bullet should travel</param>
		/// <param name="playerOwned">If the bullet was shot by the player</param>
		BulletScript(float angle, bool playerOwned = true);

		/// <summary>
		/// When the BulletScript is created
		/// </summary>
		void OnCreate() override;
		/// <summary>
		/// When the BulletScript is destroyed
		/// </summary>
		void OnDestroy() override;
		/// <summary>
		/// Move the bullets position on the screen. Check if it has surpassed its lifetime and also check
		/// for collisions with other Game Objects
		/// </summary>
		/// <param name="ts">The amount of time passed (in seconds)</param>
		void OnUpdate(Timestep ts) override;
	private:
		/// <summary>
		/// Check for collisions with asteroid Game Objects
		/// If it does collide, destroy/split the asteroid and destroy the bullet
		/// </summary>
		void CheckAsteroidCollision();
		/// <summary>
		/// Check for collisions with saucer Game Objects.
		/// If it does collide, damage the saucer and destroy the bullet
		/// </summary>
		void CheckSaucerCollision();
		/// <summary>
		/// Check for collision with the player Game Object.
		/// If it does collide, remove a life from the player and destroy the bullet
		/// </summary>
		void CheckPlayerCollision();

		/// <summary>
		/// Moves the bullet back onto the screen on the opposite side (left/right top/bottom)
		/// </summary>
		void MoveOntoScreen();

		/// <summary>
		/// Get the velocity of the bullet
		/// </summary>
		/// <returns>The velocity of the bullet</returns>
		glm::vec3 GetVelocity() const { return m_Velocity * m_Speed; }
	private:
		float m_Angle;
		glm::vec3 m_Velocity;
		float m_Lifetime = 0.0f;
		bool m_PlayerOwned;

		const glm::vec3 m_Size = { 0.1f, 0.1f, 0.1f };

		const float m_Speed = 25.0f;
		const float m_MaxLifetime = 1.25f;
	};
}