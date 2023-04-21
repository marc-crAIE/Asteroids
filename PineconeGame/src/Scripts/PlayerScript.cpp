#include "PlayerScript.h"

#include "AsteroidScript.h"
#include "BulletScript.h"
#include "GameLayer.h"
#include "Utils/Physics2D.h"

namespace AsteroidsGame
{
	void PlayerScript::OnCreate()
	{
		// Initialize the player textures
		// TODO: Move to Resources
		m_NormalTexture = Texture2D::Create("assets/textures/PlayerNormal.png");
		m_MoveTexture = Texture2D::Create("assets/textures/PlayerMove.png");

		// Set the scale and the texture
		GetComponent<TransformComponent>().Scale = glm::vec3(0.8f, 1.0f, 1.0f);
		GetComponent<SpriteComponent>().Texture = m_NormalTexture;
	}

	void PlayerScript::OnDestroy()
	{
	}

	void PlayerScript::OnUpdate(Timestep ts)
	{
		// Get the transform and sprite components
		auto& transform = GetComponent<TransformComponent>();
		auto& sprite = GetComponent<SpriteComponent>();

		// If the A key is pressed, rotate counter clockwise
		if (Input::IsKeyPressed(Key::A))
			m_Rotation += m_RotationSpeed * ts;

		// If the D key is pressed, rotate clockwise
		if (Input::IsKeyPressed(Key::D))
			m_Rotation -= m_RotationSpeed * ts;

		// If the W key is pressed
		if (Input::IsKeyPressed(Key::W))
		{
			// Change the velocity based on the rotation of the player.
			// Velocity is increased the longer the W key is pressed to accelerate the player
			m_Velocity += glm::vec3(glm::cos(glm::radians(m_Rotation)), glm::sin(glm::radians(m_Rotation)), 0.0f) * m_Speed * (float)ts;

			// Set the player texture to the move texture if it is the normal texture
			if (sprite.Texture == m_NormalTexture)
				sprite.Texture = m_MoveTexture;
		}
		else if (sprite.Texture == m_MoveTexture)
		{
			// Set the player texture back to normal if it is the move texture
			sprite.Texture = m_NormalTexture;
		}

		if (m_HyperspaceCooldownTime < m_HypespaceCooldown)
			m_HyperspaceCooldownTime += ts;

		if (Input::IsKeyPressed(Key::V))
		{
			if (m_HyperspaceCooldownTime >= m_HypespaceCooldown)
			{
				Hyperspace();
				m_HyperspaceCooldownTime = 0.0f;
			}
		}

		// If we are invulnerable, we don't want the player to be able to shoot or get damaged
		if (!m_Invulnerable)
		{
			// Get all of the bullets
			auto bullets = GetScene()->GetGameObjectsByTag("Bullet");
			// Increment the time since we last shot
			m_LastShot += ts;
			// Check if the space key is pressed, enough time has passed, and we have not surpassed 
			// the max number of bullets shot by the player
			if (Input::IsKeyPressed(Key::Space) && m_LastShot >= m_ShootCooldown && bullets.size() < m_MaxShots)
			{
				// Shoot a bullet
				ShootBullet();
				m_LastShot = 0.0f;
			}

			// Check for asteroid collision
			CheckAsteroidCollision();
		}
		else
		{
			// Increase the invulnerability timer
			m_InvulnerabilityTime += ts;

			// Check if we have been invulnerable for the max duration
			if (m_InvulnerabilityTime >= m_MaxInvulnerability)
			{
				// Turn off the players invulnerability and make sure they are visible 
				m_Invulnerable = false;
				sprite.Color.a = 1.0f;
			}
			// Here we just want to make the player flicker by changing the alpha color channel depending on time passed
			else if ((int)(m_InvulnerabilityTime * 10) % 2 == 0)
				sprite.Color.a = 0.0f;
			else
				sprite.Color.a = 1.0f;
		}

		// This line simply slows the players velocity a little bit
		m_Velocity *= m_SlowDownSpeed;
		// Modify the players position and rotation
		transform.Translation += m_Velocity;
		transform.Rotation = glm::radians(glm::vec3(0.0f, 0.0f, m_Rotation - 90.0f)); // Offset rotation by -90 degrees because of the texture

		// Keep the player on the screen
		MovePlayerOntoScreen();
	}

	void PlayerScript::ShootBullet()
	{
		// Get the players transform
		auto& transform = GetComponent<TransformComponent>();

		// Create a new bullet and set its position to where the player is
		GameObject bullet = GetScene()->CreateGameObject("Bullet");
		bullet.GetComponent<TransformComponent>().Translation = transform.Translation;
		// Add a native script component and bind it to the BulletScript passing the players rotation to the constructor
		bullet.AddComponent<NativeScriptComponent>().Bind<BulletScript>(m_Rotation);
	}

	void PlayerScript::CheckAsteroidCollision()
	{
		// Get all asteroids in the scene
		auto asteroids = GetScene()->GetGameObjectsByTag("Asteroid");
		for (GameObject asteroid : asteroids)
		{
			// Get the asteroid script
			AsteroidScript* asteroidScript = dynamic_cast<AsteroidScript*>(asteroid.GetComponent<NativeScriptComponent>().Instance);
			// Check if the player is colliding with the asteroid
			if (Physics2D::CheckCircleCollision(GetGameObject(), asteroid))
			{
				// Destroy/split the asteroid and remove a life
				asteroidScript->Destroy(false);
				RemoveLife();
				return;
			}
		}
	}

	void PlayerScript::Hyperspace()
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

		// Get the players transform
		auto& transform = GetComponent<TransformComponent>();

		// Change their position to a random point on the screen
		transform.Translation.x = rand() % (int)(orthoRight - orthoLeft + 1) + orthoLeft;
		transform.Translation.y = rand() % (int)(orthoTop - orthoBottom + 1) + orthoBottom;
	}

	void PlayerScript::AddLife()
	{
		m_Lives++;
	}

	void PlayerScript::RemoveLife()
	{
		if (m_Lives > 1)
		{
			// If the player has more than one life, decrease the number of lives
			m_Lives--;
			// Then reset their position to the center of the screen and make them invulnerable
			GetComponent<TransformComponent>().Translation = glm::vec3(0.0f);
			m_Velocity = glm::vec3(0.0f);
			m_Invulnerable = true;
			m_InvulnerabilityTime = 0.0f;
		}
		else
		{
			// Otherwise change the game state to be Game Over
			GameLayer::Get().SetState(GameState::GameOver);
		}
	}

	void PlayerScript::Reset()
	{
		// Reset the players position and rotation to 0
		auto& transform = GetComponent<TransformComponent>();
		transform.Translation = glm::vec3(0.0f);
		transform.Rotation = glm::vec3(0.0f);

		// Reset the variables to their default values
		m_Velocity = glm::vec3(0.0f);
		m_Rotation = 0.0f;
		m_Lives = 5;
		m_LastShot = m_ShootCooldown;
		m_HyperspaceCooldownTime = m_HypespaceCooldown;
		m_Invulnerable = false;
		m_InvulnerabilityTime = 0.0f;
	}

	void PlayerScript::MovePlayerOntoScreen()
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