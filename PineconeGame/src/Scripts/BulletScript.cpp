#include "BulletScript.h"

#include "AsteroidScript.h"
#include "PlayerScript.h"
#include "SaucerScript.h"
#include "GameLayer.h"
#include "Utils/Physics2D.h"

namespace AsteroidsGame
{
	BulletScript::BulletScript(float angle, bool playerOwned)
		: m_Angle(angle), m_PlayerOwned(playerOwned)
	{
		// Set the velocity based on the angle
		m_Velocity = glm::vec3(glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)), 0.0f);
	}

	void BulletScript::OnCreate()
	{
		// Adding the sprite component here stops the random "white square flash"
		AddComponent<SpriteComponent>();
		// Get the transform
		auto& transform = GetComponent<TransformComponent>();
		transform.Scale = m_Size;
		// Set the rotatation to the angle the bullet is travelling
		transform.Rotation = { 0.0f, 0.0f, glm::radians(m_Angle) };
	}

	void BulletScript::OnDestroy()
	{
	}

	void BulletScript::OnUpdate(Timestep ts)
	{
		// Get the transform and update the transform using the velocity
		auto& transform = GetComponent<TransformComponent>();
		transform.Translation += GetVelocity() * (float)ts;

		// Keep the bullet onto the screen
		MoveOntoScreen();

		// Add ts to the lifetime
		m_Lifetime += ts;
		// If the lifetime has surpassed the maximum lifetime
		if (m_Lifetime >= m_MaxLifetime)
		{
			// Destroy the game object
			GameLayer::Get().DestroyGameObject(GetGameObject(), false);
			return;
		}

		CheckAsteroidCollision();

		// If the bullet is owned by the player, check saucer collision, else check for player collision
		if (m_PlayerOwned)
			CheckSaucerCollision();
		else
			CheckPlayerCollision();
	}

	void BulletScript::CheckAsteroidCollision()
	{
		// Get all of the asteroids
		auto asteroids = GetScene()->GetGameObjectsByTag("Asteroid");
		for (GameObject asteroid : asteroids)
		{
			// Get the asteroid script
			AsteroidScript* asteroidScript = dynamic_cast<AsteroidScript*>(asteroid.GetComponent<NativeScriptComponent>().Instance);
			// Check for collision
			if (Physics2D::CheckCircleCollision(GetGameObject(), asteroid))
			{
				// Destroy the asteroid and only award a score if the bullet was shot by the player
				if (asteroidScript)
					asteroidScript->Destroy(m_PlayerOwned);

				// Destroy ourselves :(
				GameLayer::Get().DestroyGameObject(GetGameObject(), false);
				return;
			}
		}
	}

	void BulletScript::CheckSaucerCollision()
	{
		// This function should never be called if the bullet is not owned by the player.
		// Check for that and if so, exit
		if (!m_PlayerOwned)
			return;

		// Get all of the saucers
		auto saucers = GetScene()->GetGameObjectsByTag("Saucer");
		for (GameObject saucer : saucers)
		{
			// Get the saucer script
			SaucerScript* saucerScript = dynamic_cast<SaucerScript*>(saucer.GetComponent<NativeScriptComponent>().Instance);
			// Check for collision
			if (Physics2D::CheckCircleCollision(GetGameObject(), saucer))
			{
				// Damage the saucer
				if (saucerScript)
					saucerScript->Damage();

				// Destroy ourselves :(
				GameLayer::Get().DestroyGameObject(GetGameObject(), false);
				return;
			}
		}
	}

	void BulletScript::CheckPlayerCollision()
	{
		// This function should never be called if the bullet is owned by the player.
		// Check for that and if so, exit
		if (m_PlayerOwned)
			return;

		// Get the player game object
		GameObject player = GetScene()->GetGameObjectByTag("Player");
		// Check for collision
		if (Physics2D::CheckCircleCollision(GetGameObject(), player))
		{
			// Remove a life from the player
			((PlayerScript*)player.GetComponent<NativeScriptComponent>().Instance)->RemoveLife();

			// Destroy ourselves :(
			GameLayer::Get().DestroyGameObject(GetGameObject(), false);
		}
	}

	void BulletScript::MoveOntoScreen()
	{
		// Get the primary camera component
		auto& camera = GetScene()->GetPrimaryCameraGameObject().GetComponent<CameraComponent>();

		// TODO: See the comment inside UILayer::OnUpdate to know how this can be improved
		float orthoSize = camera.Camera.GetOrthographicSize();
		float aspectRatio = camera.Camera.GetAspectRatio();

		float orthoLeft = -orthoSize * aspectRatio * 0.5f;
		float orthoRight = orthoSize * aspectRatio * 0.5f;
		float orthoBottom = -orthoSize * 0.5f;
		float orthoTop = orthoSize * 0.5f;

		// Get the transform
		auto& transform = GetComponent<TransformComponent>();
		if (transform.Translation.x < orthoLeft)
		{
			// If outside the bounds on the left of the screen, move to the right
			transform.Translation.x = orthoRight;
		}
		else if (transform.Translation.x > orthoRight)
		{
			// If outside the bounds on the right of the screen, move to the left
			transform.Translation.x = orthoLeft;
		}

		if (transform.Translation.y < orthoBottom)
		{
			// If outside the bounds on the bottom of the screen, move to the top
			transform.Translation.y = orthoTop;
		}
		else if (transform.Translation.y > orthoTop)
		{
			// If outside the bounds on the top of the screen, move to the bottom
			transform.Translation.y = orthoBottom;
		}
	}
}