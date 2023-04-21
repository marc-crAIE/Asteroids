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
			// If the reference does not exist, get the player object
			auto player = GameLayer::Get().GetScene()->GetGameObjectByTag("Player");
			// Then get the instance of the script from the players NativeScriptComponent
			m_PlayerScript = (PlayerScript*)player.GetComponent<NativeScriptComponent>().Instance;
		}

		// Borrowing the camera game object from the scene. Normally would not do this if the camera moved around
		// the scene as UI elements would not follow unless we factor in the camera's translation.
		// Normally would create a seperate scene camera that just stays at a fixed position.
		auto camera = GameLayer::Get().GetScene()->GetPrimaryCameraGameObject().GetComponent<CameraComponent>().Camera;

		//TODO: Probably have a function inside of SceneCamera that returns a Bounds object to get these values
		//		as this is used in many places.

		// Get the orthographic screen bounds
		float orthoSize = camera.GetOrthographicSize();
		float aspectRatio = camera.GetAspectRatio();

		float orthoLeft = -orthoSize * aspectRatio * 0.5f;
		float orthoRight = orthoSize * aspectRatio * 0.5f;
		float orthoBottom = -orthoSize * 0.5f;
		float orthoTop = orthoSize * 0.5f;

		// Begin the 2D renderer as we want to draw some stuff in 2D
		// Note: This kinda sucks at the moment as everytime Renderer2D::BeginScene is called it does at least 1 new draw call.
		//		 A TODO would be to have UI components and create a UI system in the engine itself so they can be drawn inside of the scene
		Renderer2D::BeginScene(camera);

		// Call the correct draw function depending on the game state in our GameLayer
		switch (GameLayer::Get().GetState())
		{
		case GameState::Playing:
			DrawGameUI(glm::vec4(orthoLeft, orthoRight, orthoTop, orthoBottom));
			break;
		case GameState::MainMenu:
			DrawMainMenu(glm::vec4(orthoLeft, orthoRight, orthoTop, orthoBottom));
			break;
		case GameState::SubmitHighscore:
			DrawSubmitHighscore(glm::vec4(orthoLeft, orthoRight, orthoTop, orthoBottom));
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

	void UILayer::DrawGameUI(glm::vec4& screenDimensions)
	{
		// Fixed values for configuring the UI
		const float scale = 1.5f;		// The overall scale of the UI elements
		const float scoreHeight = 1.0f;	// The height (scale) of the score text (change the 1.0f to scale only the text)
		const float offset = 1.0f;		// How far to offset the UI from the edge of the screen
		const float separation = 0.25f; // The spacing between the UI elements

		// The x and y position for where to start drawing the UI
		float xPos = screenDimensions.x + offset;
		float yPos = screenDimensions.z - offset;

		// A vec3 of the size of the player life texture. This is chosen from the size of the texture width and height
		glm::vec3 lifeScale = glm::vec3(0.8f, 1.0f, 1.0f) * 0.75f * scale;

		// Draw the score text
		DrawString(std::to_string(GameLayer::Get().GetScore()), glm::vec2(xPos - 0.25f, yPos), glm::vec2(scoreHeight * scale));

		// Draw all of the lives
		for (int i = 0; i < m_PlayerScript->GetLives(); i++)
		{
			// Get the position to render the player life texture at
			glm::vec3 pos = glm::vec3(xPos + ((lifeScale.x + separation) * i), yPos - ((scoreHeight * scale) / 2 + separation), 1.0f);
			// Create the transform using the position vec3 and the scale vec3
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
				* glm::scale(glm::mat4(1.0f), lifeScale);
			// Draw a textured quad
			Renderer2D::DrawQuad(transform, Resources::GetPlayerLifeTexture());
		}
	}

	void UILayer::DrawMainMenu(glm::vec4& screenDimensions)
	{
		// Draw a bunch of strings for the main menu.
		// The position of the strings is picked on what looks right
		// Another TODO would be to have the Font class have something like a MeasureText function that takes
		// in a string, font, and the scale to get the vec2 size of the font when drawn.

		DrawString("Highscores", glm::vec2(screenDimensions.x + 0.5f, screenDimensions.z - 1.5f), glm::vec2(2.0f));

		auto scores = GameLayer::Get().GetScores();
		for (int i = 0; i < 3; i++)
		{
			if (i < scores.size())
			{
				std::string str = scores[i].Name + ": " + std::to_string(scores[i].Score);
				DrawString(str, glm::vec2(screenDimensions.x + 0.5f, screenDimensions.z - 2.5f - i), glm::vec2(1.25f), glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
			}
		}		

		DrawString("Asteroids", glm::vec2(-6.0f, 0.0f), glm::vec2(3.0f));
		DrawString("Press the spacebar to play", glm::vec2(-7.5f, -1.25f), glm::vec2(1.25f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

		DrawString("Controls", glm::vec2(-2.75f, -5.0f), glm::vec2(1.5f), glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
		DrawString("A and D to rotate", glm::vec2(-5.35f, -6.25f), glm::vec2(1.25f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
		DrawString("W to thrust", glm::vec2(-3.25f, -7.5f), glm::vec2(1.25f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
		DrawString("SPACE to shoot", glm::vec2(-4.25f, -8.75f), glm::vec2(1.25f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
		DrawString("V to hyperspace", glm::vec2(-4.65f, -10.0f), glm::vec2(1.25f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	void UILayer::DrawSubmitHighscore(glm::vec4& screenDimensions)
	{
		DrawString(std::to_string(GameLayer::Get().GetScore()), glm::vec2(screenDimensions.x + 5.0f, 7.5f), glm::vec2(2.0f));
		DrawString("New Highscore!", glm::vec2(screenDimensions.x + 5.0f, 5.0f), glm::vec2(2.0f));
		DrawString("Type your name, press enter to submit score", glm::vec2(screenDimensions.x + 5.0f, 2.5f), glm::vec2(1.5f), glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
		DrawString(GameLayer::Get().GetPlayerName(), glm::vec2(screenDimensions.x + 5.0f, 0.0f), glm::vec2(2.0f));
	}

	void UILayer::DrawGameOver(glm::vec4& screenDimensions)
	{
		// Draw a bunch of strings for the game over screen.
		// Refer to the DrawMainMenu for more details

		DrawString("Game Over", glm::vec2(-6.0f, 0.0f), glm::vec2(3.0f));
		DrawString("Press the spacebar to replay", glm::vec2(-8.0f, -1.25f), glm::vec2(1.25f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
		DrawString("Press the escape key to exit", glm::vec2(-8.0f, -2.5f), glm::vec2(1.25f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	void UILayer::DrawPaused(glm::vec4& screenDimensions)
	{
		// Draw a bunch of strings for the paused screen.
		// Refer to the DrawMainMenu for more details

		DrawString("Paused", glm::vec2(-3.5f, 0.0f), glm::vec2(3.0f));
		DrawString("Press the escape key to resume", glm::vec2(-8.0f, -1.25f), glm::vec2(1.25f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	void UILayer::DrawString(const std::string& text, glm::vec2& position, glm::vec2& scale, glm::vec4& color)
	{
		// Craete a transform based on the position and scale
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));
		// Draw the string using our font
		Renderer2D::DrawString(text, m_Font, transform, color);
	}
}