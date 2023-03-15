#include "BulletScript.h"

#include "AsteroidScript.h"
#include "Utils/Physics2D.h"

namespace AsteroidsGame
{
	BulletScript::BulletScript(float angle)
		: m_Angle(angle)
	{
		m_Velocity = glm::vec3(glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)), 0.0f);
	}

	void BulletScript::OnCreate()
	{
		AddComponent<SpriteComponent>();
		auto& transform = GetComponent<TransformComponent>();
		transform.Scale = m_Size;
		transform.Rotation = { 0.0f, 0.0f, glm::radians(m_Angle) };
	}

	void BulletScript::OnDestroy()
	{
	}

	void BulletScript::OnUpdate(Timestep ts)
	{
		auto& transform = GetComponent<TransformComponent>();

		transform.Translation += GetVelocity() * (float)ts;

		MoveOntoScreen();

		m_Lifetime += ts;
		if (m_Lifetime >= m_MaxLifetime)
		{
			GetScene()->DestroyGameObject(GetGameObject());
			return;
		}

		CheckAsteroidCollision(ts);
	}

	void BulletScript::CheckAsteroidCollision(Timestep ts)
	{
		auto asteroids = GetScene()->GetGameObjectsByTag("Asteroid");
		for (GameObject asteroid : asteroids)
		{
			AsteroidScript* asteroidScript = (AsteroidScript*)asteroid.GetComponent<NativeScriptComponent>().Instance;
			if (Physics2D::CheckCircleCollision(GetGameObject(), asteroid))//m_Velocity * m_Speed * (float)ts, asteroidScript->GetVelocity() * (float)ts))
			{
				asteroidScript->Destroy();

				GetScene()->DestroyGameObject(GetGameObject());
				return;
			}
		}
	}

	void BulletScript::MoveOntoScreen()
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