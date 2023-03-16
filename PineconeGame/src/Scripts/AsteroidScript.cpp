#include "AsteroidScript.h"

#include "GameLayer.h"
#include "Utils/Resources.h"

#include <random>

namespace AsteroidsGame
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<int> s_UniformDistribution(0, 360);

	AsteroidScript::AsteroidScript(float angle)
		: m_Angle(angle)
	{
		m_Velocity = glm::vec3(glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)), 0.0f);
		m_Size = (Size)(rand() % (Size::Large - Size::Small + 1) + Size::Small);
		m_Speed = m_Speed + (m_SpeedIncAmount * (Size::Large - m_Size));
	}

	AsteroidScript::AsteroidScript(float angle, Size size)
		: m_Angle(angle), m_Size(size)
	{
		m_Velocity = glm::vec3(glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)), 0.0f);
		m_Speed = m_Speed + (m_SpeedIncAmount * (Size::Large - m_Size));
	}

	void AsteroidScript::OnCreate()
	{
		AddComponent<SpriteComponent>();
		auto& transform = GetComponent<TransformComponent>();
		transform.Rotation = glm::vec3(0.0f, 0.0f, s_UniformDistribution(s_Engine));
		transform.Scale = glm::vec3(1.0f + (0.5f * m_Size));
		GetComponent<SpriteComponent>().Texture = Resources::GetAsteroidTexture();
	}

	void AsteroidScript::OnDestroy()
	{
	}

	void AsteroidScript::OnUpdate(Timestep ts)
	{
		auto& transform = GetComponent<TransformComponent>();
		transform.Translation += GetVelocity() * (float)ts;

		MoveOntoScreen();
	}

	void AsteroidScript::Destroy()
	{
		if (m_Size != Size::Small)
			Split();

		// Have to make the game layer destroy the asteroid after the update loop for the native scripts.
		// Destroying the asteroid by calling "GetScene()->DestroyGameObject(GetGameObject());" seems to cause
		// issues with the entt library entity handle ID for the bullet objects. Not exactly sure what is 
		// causing the issue specifically but this seems to fix it.
		GameLayer::Get().DestroyGameObject(GetGameObject());
	}

	void AsteroidScript::Split()
	{
		GameObject asteroidLeft = GetScene()->CreateGameObject("Asteroid");
		GameObject asteroidRight = GetScene()->CreateGameObject("Asteroid");

		auto& transform = GetComponent<TransformComponent>();

		asteroidLeft.GetComponent<TransformComponent>().Translation = transform.Translation;
		asteroidRight.GetComponent<TransformComponent>().Translation = transform.Translation;

		float splitAngle = rand() % (m_MaxSplitAngle - m_MinSplitAngle + 1) + m_MinSplitAngle;

		asteroidLeft.AddComponent<NativeScriptComponent>().Bind<AsteroidScript>(m_Angle - splitAngle, (Size)(m_Size - 1));
		asteroidRight.AddComponent<NativeScriptComponent>().Bind<AsteroidScript>(m_Angle + splitAngle, (Size)(m_Size - 1));
	}

	void AsteroidScript::MoveOntoScreen()
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