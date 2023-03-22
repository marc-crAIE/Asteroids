#include "AsteroidScript.h"

#include "GameLayer.h"
#include "Utils/Resources.h"

#include <random>

namespace AsteroidsGame
{
	static std::mt19937 s_Engine;
	static std::uniform_int_distribution<int> s_UniformDistribution(0, 360);

	AsteroidScript::AsteroidScript(float angle)
		: m_Angle(angle)
	{
		// Set the velocity based on the angle, pick a random size, and set the speed based on the size
		m_Velocity = glm::vec3(glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)), 0.0f);
		m_Size = (Size)(rand() % (Size::Large - Size::Medium + 1) + Size::Medium);
		m_Speed = m_Speed + (m_SpeedIncAmount * (Size::Large - m_Size));
	}

	AsteroidScript::AsteroidScript(float angle, Size size)
		: m_Angle(angle), m_Size(size)
	{
		// Set the velocity based on the angle and set the speed based on the size
		m_Velocity = glm::vec3(glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)), 0.0f);
		m_Speed = m_Speed + (m_SpeedIncAmount * (Size::Large - m_Size));
	}

	void AsteroidScript::OnCreate()
	{
		// Add the sprite component and set the texture to a random asteroid texture
		AddComponent<SpriteComponent>();
		GetComponent<SpriteComponent>().Texture = Resources::GetAsteroidTexture();

		// Get the transform
		auto& transform = GetComponent<TransformComponent>();
		// Set a random rotation and scale based on the size
		transform.Rotation = glm::vec3(0.0f, 0.0f, s_UniformDistribution(s_Engine));
		transform.Scale = glm::vec3(1.0f + (0.5f * m_Size));
	}

	void AsteroidScript::OnDestroy()
	{

	}

	void AsteroidScript::OnUpdate(Timestep ts)
	{
		// Get the transform and update the translation based on the velocity
		auto& transform = GetComponent<TransformComponent>();
		transform.Translation += GetVelocity() * (float)ts;

		// Keep the asteroid on the screen
		MoveOntoScreen();
	}

	void AsteroidScript::Destroy(bool awardScore)
	{
		// If the size is not small, split the asteroid
		if (m_Size != Size::Small)
			Split();

		if (awardScore)
		{
			// If we are awarding the score, add to the score based on the size of the asteroid
			switch (m_Size)
			{
			case Size::Small:
				GameLayer::Get().IncreaseScore(100);
				break;
			case Size::Medium:
				GameLayer::Get().IncreaseScore(50);
				break;
			case Size::Large:
				GameLayer::Get().IncreaseScore(20);
				break;
			}
		}

		// Have to make the game layer destroy the asteroid after the update loop for the native scripts.
		// Destroying the asteroid by calling "GetScene()->DestroyGameObject(GetGameObject());" seems to cause
		// issues with the entt library entity handle ID for the bullet objects. Not exactly sure what is 
		// causing the issue specifically but this seems to fix it.
		// TODO: Find a fix inside the engine
		GameLayer::Get().DestroyGameObject(GetGameObject());
	}

	void AsteroidScript::Split()
	{
		// CHeck if the size is the smallest size. If so exit the function
		if (m_Size == Size::Small)
			return;

		// Create two asteroid Game Objects
		GameObject asteroidLeft = GetScene()->CreateGameObject("Asteroid");
		GameObject asteroidRight = GetScene()->CreateGameObject("Asteroid");

		// Get our transform and set the translation of the two asteroids to be the same
		auto& transform = GetComponent<TransformComponent>();

		asteroidLeft.GetComponent<TransformComponent>().Translation = transform.Translation;
		asteroidRight.GetComponent<TransformComponent>().Translation = transform.Translation;

		// Get a random angle to have the two asteroids move away from each other
		float splitAngle = rand() % (m_MaxSplitAngle - m_MinSplitAngle + 1) + m_MinSplitAngle;

		// Add the Native Script components, pass the split angle and a size smaller than ours to the constructor
		asteroidLeft.AddComponent<NativeScriptComponent>().Bind<AsteroidScript>(m_Angle - splitAngle, (Size)(m_Size - 1));
		asteroidRight.AddComponent<NativeScriptComponent>().Bind<AsteroidScript>(m_Angle + splitAngle, (Size)(m_Size - 1));
	}

	void AsteroidScript::MoveOntoScreen()
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