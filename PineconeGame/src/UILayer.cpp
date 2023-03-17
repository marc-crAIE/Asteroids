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

		DrawLivesAndScore(glm::vec4(orthoLeft, orthoRight, orthoTop, orthoBottom));

		// End the 2D renderer
		Renderer2D::EndScene();
	}

	void UILayer::OnEvent(Event& e)
	{
	}

	void UILayer::DrawLivesAndScore(glm::vec4& screenDimensions)
	{
		const float scoreHeight = 1.0f;
		const float offset = 1.0f;
		const float separation = 0.25f;
		float xPos = screenDimensions.x + offset;
		float yPos = screenDimensions.z - offset;

		glm::vec3 scale = glm::vec3(0.8f, 1.0f, 1.0f) * 0.75f;

		glm::mat4 scorePos = glm::translate(glm::mat4(1.0f), glm::vec3(xPos - 0.25f, yPos, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(scoreHeight));
		Renderer2D::DrawString(std::to_string(GameLayer::Get().GetScore()), m_Font, scorePos, glm::vec4(1.0f));

		for (int i = 0; i < m_PlayerScript->GetLives(); i++)
		{
			glm::vec3 pos = glm::vec3(xPos + ((scale.x + separation) * i), yPos - (scoreHeight / 2 + separation), 1.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
				* glm::scale(glm::mat4(1.0f), scale);
			Renderer2D::DrawQuad(transform, Resources::GetPlayerLifeTexture());
		}
	}
}