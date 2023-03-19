#include "UILayer.h"

#include "GameLayer.h"
#include "Utils/Resources.h"

namespace AsteroidsGame
{
	UILayer::UILayer() : Layer("UILayer")
	{
		m_Font = Font::Create("assets/fonts/hyperspace/Hyperspace-Bold.ttf");
	}

	void UILayer::OnAttach()
	{
	}

	void UILayer::OnDetach()
	{
	}

	void UILayer::OnUpdate(Timestep ts)
	{
		// Create a reference to the player script
		if (m_PlayerScript == nullptr)
		{
			auto player = GameLayer::Get().GetScene()->GetGameObjectByTag("Player");
			m_PlayerScript = (PlayerScript*)player.GetComponent<NativeScriptComponent>().Instance;
		}

		// Borrowing the camera game object from the scene. Normally would not do this if the camera moved around
		// the scene as UI elements would not follow unless we factor in the camera's translation.
		// Normally would create a seperate scene camera that just stays at a fixed position.
		auto camera = GameLayer::Get().GetScene()->GetPrimaryCameraGameObject().GetComponent<CameraComponent>().Camera;

		float orthoSize = camera.GetOrthographicSize();
		float aspectRatio = camera.GetAspectRatio();

		float orthoLeft = -orthoSize * aspectRatio * 0.5f;
		float orthoRight = orthoSize * aspectRatio * 0.5f;
		float orthoBottom = -orthoSize * 0.5f;
		float orthoTop = orthoSize * 0.5f;

		// Begin the 2D renderer as we want to draw some stuff in 2D
		Renderer2D::BeginScene(camera);

		switch (GameLayer::Get().GetState())
		{
		case GameState::Playing:
			DrawLivesAndScore(glm::vec4(orthoLeft, orthoRight, orthoTop, orthoBottom));
			break;
		case GameState::MainMenu:
			DrawMainMenu(glm::vec4(orthoLeft, orthoRight, orthoTop, orthoBottom));
			break;
		case GameState::GameOver:
			DrawGameOver(glm::vec4(orthoLeft, orthoRight, orthoTop, orthoBottom));
			break;
		case GameState::Paused:
			DrawPaused(glm::vec4(orthoLeft, orthoRight, orthoTop, orthoBottom));
			break;
		}

		// End the 2D renderer
		Renderer2D::EndScene();
	}

	void UILayer::OnEvent(Event& e)
	{
	}

	void UILayer::DrawGameUI(glm::vec4& screenDimensions)
	{
		DrawLivesAndScore(screenDimensions);
	}

	void UILayer::DrawMainMenu(glm::vec4& screenDimensions)
	{
		DrawString("Asteroids", glm::vec2(-6.0f, 0.0f), glm::vec2(3.0f));
		DrawString("Press the spacebar to play", glm::vec2(-7.5f, -1.25f), glm::vec2(1.25f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

		DrawString("Controls", glm::vec2(-2.75f, -5.0f), glm::vec2(1.5f), glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
		DrawString("A and D to rotate", glm::vec2(-5.35f, -6.25f), glm::vec2(1.25f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
		DrawString("W to thrust", glm::vec2(-3.25f, -7.5f), glm::vec2(1.25f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
		DrawString("SPACE to shoot", glm::vec2(-4.25f, -8.75f), glm::vec2(1.25f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
		DrawString("V to hyperspace", glm::vec2(-4.65f, -10.0f), glm::vec2(1.25f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	void UILayer::DrawGameOver(glm::vec4& screenDimensions)
	{
		DrawString("Game Over", glm::vec2(-6.0f, 0.0f), glm::vec2(3.0f));
		DrawString("Press the spacebar to replay", glm::vec2(-8.0f, -1.25f), glm::vec2(1.25f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
		DrawString("Press the escape key to exit", glm::vec2(-8.0f, -2.5f), glm::vec2(1.25f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	void UILayer::DrawPaused(glm::vec4& screenDimensions)
	{
		DrawString("Paused", glm::vec2(-3.5f, 0.0f), glm::vec2(3.0f));
		DrawString("Press the escape key to resume", glm::vec2(-8.0f, -1.25f), glm::vec2(1.25f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	void UILayer::DrawLivesAndScore(glm::vec4& screenDimensions)
	{
		const float scale = 1.5f;
		const float scoreHeight = 1.0f * scale;
		const float offset = 1.0f;
		const float separation = 0.25f;

		float xPos = screenDimensions.x + offset;
		float yPos = screenDimensions.z - offset;

		glm::vec3 lifeScale = glm::vec3(0.8f, 1.0f, 1.0f) * 0.75f * scale;

		DrawString(std::to_string(GameLayer::Get().GetScore()), glm::vec2(xPos - 0.25f, yPos), glm::vec2(scoreHeight));

		for (int i = 0; i < m_PlayerScript->GetLives(); i++)
		{
			glm::vec3 pos = glm::vec3(xPos + ((lifeScale.x + separation) * i), yPos - (scoreHeight / 2 + separation), 1.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
				* glm::scale(glm::mat4(1.0f), lifeScale);
			Renderer2D::DrawQuad(transform, Resources::GetPlayerLifeTexture());
		}
	}

	void UILayer::DrawString(const std::string& text, glm::vec2& position, glm::vec2& scale, glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));
		Renderer2D::DrawString(text, m_Font, transform, color);
	}
}