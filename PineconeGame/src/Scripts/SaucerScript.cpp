#include "SaucerScript.h"

#include "GameLayer.h"
#include "BulletScript.h"
#include "Utils/Resources.h"

#include <random>

namespace AsteroidsGame
{
	std::mt19937 s_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;

	SaucerScript::SaucerScript(Type type)
		: m_Type(type)
	{
	}

	void SaucerScript::OnCreate()
	{
		// Add a sprite component and set the texture
		auto& sprite = AddComponent<SpriteComponent>();
		sprite.Texture = Resources::GetSaucerTexture();

		// Set the scale based on the size of the texture itself and modify based on the type
		GetComponent<TransformComponent>().Scale = glm::vec3(1.0f, 0.57, 1.0f) 
			* (m_Type == Type::Small ? m_SmallSaucerSize : m_LargeSaucerSize);

		if (m_Type == Type::Small)
		{
			// If the type is small, set the velocity x direction to be positive or negative depending on
			// what side of the screen it spawned. We only care about the x velocity as the small saucer moves 
			// horizontally only.
			if (GetComponent<TransformComponent>().Translation.x > 0)
			{
				m_Velocity.x = m_Speed;
			}
			else
			{
				m_Velocity.x = -m_Speed;
			}
		}
		else
		{
			// Pick a random direction change time and a random angle
			m_ChangeDirectionTime = ((float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<int32_t>::max())
				* (m_MaxChangeDirectionTime - m_MinChangeDirectionTime + 1) + m_MinChangeDirectionTime;
			float angle = glm::radians((float)(s_Distribution(s_RandomEngine) % 360));

			// Set the velocity based on the random angle picked
			m_Velocity = glm::vec3(glm::cos(angle), glm::sin(angle), 0.0f) * m_Speed;
		}
	}

	void SaucerScript::OnDestroy()
	{
	}

	void SaucerScript::OnUpdate(Timestep ts)
	{
		// Get the transform component and update the position based on the velocity
		auto& transform = GetComponent<TransformComponent>();
		transform.Translation += m_Velocity * (float)ts;

		// If we have not shot in a little while, shoot again
		if (m_ShootTime >= m_MaxShootTime)
		{
			m_ShootTime = 0.0f;
			Shoot();
		}
		m_ShootTime += ts;

		// Only the large saucer type can wander around
		if (m_Type == Type::Large)
			Wander(ts);

		// Keep the saucer on the screen
		MoveOntoScreen();
	}

	void SaucerScript::Damage(bool awardScore)
	{
		// If we can award a score to the player, do so according to the type
		if (awardScore)
		{
			switch (m_Type)
			{
			case Type::Small:
				GameLayer::Get().IncreaseScore(1000);
				break;
			case Type::Large:
				GameLayer::Get().IncreaseScore(200);
				break;
			}
		}

		// Destroy ourselves :(
		GameLayer::Get().DestroyGameObject(GetGameObject());
	}

	void SaucerScript::Shoot()
	{
		// By default we will pick a random angle to shoot at as that is what the large saucer does
		float angle = (float)(s_Distribution(s_RandomEngine) % 360);
		auto& transform = GetComponent<TransformComponent>();

		// Only the small saucers shoot accurately, large ones shoot randomly
		if (m_Type == Type::Small)
		{
			auto& player = GetScene()->GetGameObjectByTag("Player");
			auto& playerTransform = player.GetComponent<TransformComponent>();

			// Calculate the angle in which the player is at relative to ourselves
			// Note: Make sure it is in degrees as the bullet script uses degrees for its angle
			angle = glm::degrees(atan2(playerTransform.Translation.y - transform.Translation.y, 
				playerTransform.Translation.x - transform.Translation.x));
		}

		// Create a new bullet and set its poisition to where we are
		// Note: The tag of this bullet is set as EnemyBullet, this is done so that when the player script gets 
		//		 the bullets shot by the player itself. It does not also get the bullets shot by the saucers
		GameObject bullet = GetScene()->CreateGameObject("EnemyBullet");
		bullet.GetComponent<TransformComponent>().Translation = transform.Translation;
		// Add a native script conponent and bind it to BulletScript passing our angle and setting it as not owned by the player
		bullet.AddComponent<NativeScriptComponent>().Bind<BulletScript>(angle, false);
	}

	void SaucerScript::Wander(Timestep ts)
	{
		if (m_ChangeDirectionElapsed >= m_ChangeDirectionTime)
		{
			// If enough time has passed for us to change direction, pick a random new direction change.
			m_ChangeDirectionTime = ((float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<int32_t>::max())
				* (m_MaxChangeDirectionTime - m_MinChangeDirectionTime + 1) + m_MinChangeDirectionTime;

			// Then pick a new random angle (could by chance even be the same angle)
			float angle = glm::radians((float)(s_Distribution(s_RandomEngine) % 360));

			// Then set the new velocity
			m_Velocity = glm::vec3(glm::cos(angle), glm::sin(angle), 0.0f) * m_Speed;
			m_ChangeDirectionElapsed = 0.0f;
		}
		m_ChangeDirectionElapsed += ts;
	}

	void SaucerScript::MoveOntoScreen()
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
