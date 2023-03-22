#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class PlayerScript : public ScriptableGameObject
	{
	public:
		/// <summary>
		/// When the PlayerScript is created
		/// </summary>
		void OnCreate() override;
		/// <summary>
		/// When the PlayerScript is destroyed
		/// </summary>
		void OnDestroy() override;
		/// <summary>
		/// Update the player position and change the players velocity based on key inputs.
		/// Also checks for collision with other objects and shoots bullets
		/// </summary>
		/// <param name="ts">The amount of time passed (in seconds)</param>
		void OnUpdate(Timestep ts) override;

		/// <summary>
		/// Add a life to the player
		/// </summary>
		void AddLife();
		/// <summary>
		/// Remove a life from the player and make the player invulnerable.
		/// Also checks if the player has died and if so, ends the game
		/// </summary>
		void RemoveLife();

		/// <summary>
		/// Resets the players values to default and resets their position to the center of the screen
		/// </summary>
		void Reset();

		/// <summary>
		/// Get the amount of lives the player has
		/// </summary>
		/// <returns>The amount of lives the player has</returns>
		int GetLives() const { return m_Lives; }
	private:
		/// <summary>
		/// Shoots a bullet in the players forward direction
		/// </summary>
		void ShootBullet();
		/// <summary>
		/// Checks for collision with asteroids.
		/// Removes a life and destroys/splits the asteroid if colliding
		/// </summary>
		void CheckAsteroidCollision();
		/// <summary>
		/// Teleport the player to a random spot on the screen
		/// </summary>
		void Hyperspace();

		/// <summary>
		/// Moves the player back onto the screen on the opposite side (left/right top/bottom)
		/// </summary>
		void MovePlayerOntoScreen();
	private:
		Ref<Texture2D> m_NormalTexture;
		Ref<Texture2D> m_MoveTexture;

		glm::vec3 m_Velocity = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
		int m_Lives = 5;

		bool m_Invulnerable = false;
		float m_InvulnerabilityTime = 0.0f;
		const float m_MaxInvulnerability = 2.5f;

		const float m_HypespaceCooldown = 1.0f;
		float m_HyperspaceCooldownTime = m_HypespaceCooldown;

		const float m_Speed = 0.2f;
		const float m_RotationSpeed = 240.0f;
		const float m_SlowDownSpeed = 0.994f;

		const int m_MaxShots = 4;
		const float m_ShootCooldown = 0.4f;
		float m_LastShot = m_ShootCooldown;
	};
}