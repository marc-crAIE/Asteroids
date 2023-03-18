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
		auto& sprite = AddComponent<SpriteComponent>();
		sprite.Texture = Resources::GetSaucerTexture();

		GetComponent<TransformComponent>().Scale = glm::vec3(1.0f, 0.57, 1.0f) 
			* (m_Type == Type::Small ? m_SmallSaucerSize : m_LargeSaucerSize);

		if (m_Type == Type::Small)
		{
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
			m_ChangeDirectionTime = ((float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<int32_t>::max())
				* (m_MaxChangeDirectionTime - m_MinChangeDirectionTime + 1) + m_MinChangeDirectionTime;
			float angle = glm::radians((float)(s_Distribution(s_RandomEngine) % 360));
			m_Velocity = glm::vec3(glm::cos(angle), glm::sin(angle), 0.0f) * m_Speed;
		}
	}

	void SaucerScript::OnDestroy()
	{
	}

	void SaucerScript::OnUpdate(Timestep ts)
	{
		auto& transform = GetComponent<TransformComponent>();
		transform.Translation += m_Velocity * (float)ts;

		if (m_ShootTime >= m_MaxShootTime)
		{
			m_ShootTime = 0.0f;
			Shoot();
		}
		m_ShootTime += ts;

		if (m_Type == Type::Large)
			Wander(ts);

		MoveOntoScreen();
	}

	void SaucerScript::Damage(bool awardScore)
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

		GameLayer::Get().DestroyGameObject(GetGameObject());
	}

	void SaucerScript::Shoot()
	{
		float angle = (float)(s_Distribution(s_RandomEngine) % 360);
		auto& transform = GetComponent<TransformComponent>();

		// Only the small saucers shoot accurately, large ones shoot randomly
		if (m_Type == Type::Small)
		{
			auto& player = GetScene()->GetGameObjectByTag("Player");
			auto& playerTransform = player.GetComponent<TransformComponent>();

			angle = atan2(playerTransform.Translation.y - transform.Translation.y, playerTransform.Translation.x - transform.Translation.x);
		}

		GameObject bullet = GetScene()->CreateGameObject("EnemyBullet");
		bullet.GetComponent<TransformComponent>().Translation = transform.Translation;
		bullet.AddComponent<NativeScriptComponent>().Bind<BulletScript>(glm::degrees(angle), false);
	}

	void SaucerScript::Wander(Timestep ts)
	{
		if (m_ChangeDirectionElapsed >= m_ChangeDirectionTime)
		{
			m_ChangeDirectionTime = ((float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<int32_t>::max())
				* (m_MaxChangeDirectionTime - m_MinChangeDirectionTime + 1) + m_MinChangeDirectionTime;
			float angle = glm::radians((float)(s_Distribution(s_RandomEngine) % 360));
			m_Velocity = glm::vec3(glm::cos(angle), glm::sin(angle), 0.0f) * m_Speed;
			m_ChangeDirectionElapsed = 0.0f;
		}
		m_ChangeDirectionElapsed += ts;
	}

	void SaucerScript::MoveOntoScreen()
	{
		auto& camera = GetScene()->GetPrimaryCameraGameObject().GetComponent<CameraComponent>();

		float orthoSize = camera.Camera.GetOrthographicSize();
		float aspectRatio = camera.Camera.GetAspectRatio();

		float orthoLeft = -orthoSize * aspectRatio * 0.5f;
		float orthoRight = orthoSize * aspectRatio * 0.5f;
		float orthoBottom = -orthoSize * 0.5f;
		float orthoTop = orthoSize * 0.5f;

		auto& transform = GetComponent<TransformComponent>();

		if (transform.Translation.x < orthoLeft)
		{
			transform.Translation.x = orthoRight;
		}
		else if (transform.Translation.x > orthoRight)
		{
			transform.Translation.x = orthoLeft;
		}

		if (transform.Translation.y < orthoBottom)
		{
			transform.Translation.y = orthoTop;
		}
		else if (transform.Translation.y > orthoTop)
		{
			transform.Translation.y = orthoBottom;
		}
	}
}
