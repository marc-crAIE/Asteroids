#include "SaucerScript.h"

#include "Utils/Resources.h"

namespace AsteroidsGame
{
	SaucerScript::SaucerScript(Type type)
		: m_Type(type)
	{
	}

	void SaucerScript::OnCreate()
	{
		auto& sprite = AddComponent<SpriteComponent>();
		sprite.Texture = Resources::GetSaucerTexture();

		GetComponent<TransformComponent>().Scale = glm::vec3(1.0f, 0.57, 1.0f) * 2.0f;

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
	}

	void SaucerScript::OnDestroy()
	{
	}

	void SaucerScript::OnUpdate(Timestep ts)
	{
		auto& transform = GetComponent<TransformComponent>();
		transform.Translation += m_Velocity * (float)ts;

		MoveOntoScreen();
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
