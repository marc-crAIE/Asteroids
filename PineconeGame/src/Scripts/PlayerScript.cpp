#include "PlayerScript.h"

#include "BulletScript.h"

namespace AsteroidsGame
{
	void PlayerScript::OnCreate()
	{
		m_NormalTexture = Texture2D::Create("assets/textures/PlayerNormal.png");
		m_MoveTexture = Texture2D::Create("assets/textures/PlayerMove.png");

		GetComponent<TransformComponent>().Scale = glm::vec3(0.8f, 1.0f, 1.0f);
		GetComponent<SpriteComponent>().Texture = m_NormalTexture;
	}

	void PlayerScript::OnDestroy()
	{
	}

	void PlayerScript::OnUpdate(Timestep ts)
	{
		auto& transform = GetComponent<TransformComponent>();
		auto& sprite = GetComponent<SpriteComponent>();

		if (Input::IsKeyPressed(Key::W))
		{
			m_Velocity += glm::vec3(glm::cos(glm::radians(m_Rotation)), glm::sin(glm::radians(m_Rotation)), 0.0f) * m_Speed * (float)ts;

			if (sprite.Texture == m_NormalTexture)
				sprite.Texture = m_MoveTexture;
		}
		else if (sprite.Texture == m_MoveTexture)
		{
			sprite.Texture = m_NormalTexture;
		}

		if (Input::IsKeyPressed(Key::A))
			m_Rotation += m_RotationSpeed * ts;

		if (Input::IsKeyPressed(Key::D))
			m_Rotation -= m_RotationSpeed * ts;

		m_LastShot += ts;
		if (Input::IsKeyPressed(Key::Space) && m_LastShot >= m_ShootCooldown)
		{
			ShootBullet();
			m_LastShot = 0.0f;
		}

		m_Velocity *= m_SlowDownSpeed;
		transform.Translation += m_Velocity;
		transform.Rotation = glm::radians(glm::vec3(0.0f, 0.0f, m_Rotation - 90.0f)); // Offset rotation by -90 degrees because of the texture

		MovePlayerOntoScreen();
	}

	void PlayerScript::ShootBullet()
	{
		auto& transform = GetComponent<TransformComponent>();

		GameObject bullet = GetScene()->CreateGameObject("Bullet");
		bullet.AddComponent<SpriteComponent>();
		bullet.GetComponent<TransformComponent>().Translation = transform.Translation;
		bullet.AddComponent<NativeScriptComponent>().Bind<BulletScript>(m_Rotation);
	}

	void PlayerScript::MovePlayerOntoScreen()
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