#pragma once

#include <Pinecone.h>

#include "Scripts/PlayerScript.h"

using namespace Pinecone;

namespace AsteroidsGame
{
	class UILayer : public Layer
	{
	public:
		/// <summary>
		/// The UILayer constructor
		/// </summary>
		UILayer();
		/// <summary>
		/// The UILayer destructor
		/// </summary>
		~UILayer() = default;

		/// <summary>
		/// Called when the UI Layer is attached
		/// </summary>
		void OnAttach() override;
		/// <summary>
		/// Called when the UI Layer is detached
		/// </summary>
		void OnDetach() override;

		/// <summary>
		/// Update the UI and display the correct info based on the game state
		/// </summary>
		/// <param name="ts">The amount of time passed (in seconds)</param>
		void OnUpdate(Timestep ts) override;
	private:
		/// <summary>
		/// Draw the UI for when the game is in a playing state
		/// </summary>
		/// <param name="screenDimensions"></param>
		void DrawGameUI(glm::vec4& screenDimensions);
		/// <summary>
		/// Draw the UI for when the game is in a main menu state
		/// </summary>
		/// <param name="screenDimensions"></param>
		void DrawMainMenu(glm::vec4& screenDimensions);
		/// <summary>
		/// Draw the UI for when the game is in a submit highscore state
		/// </summary>
		/// <param name="screenDimensions"></param>
		void DrawSubmitHighscore(glm::vec4& screenDimensions);
		/// <summary>
		/// Draw the UI for when the game is in a game over state
		/// </summary>
		/// <param name="screenDimensions"></param>
		void DrawGameOver(glm::vec4& screenDimensions);
		/// <summary>
		/// Draw the UI for when the game is in a paused state
		/// </summary>
		/// <param name="screenDimensions"></param>
		void DrawPaused(glm::vec4& screenDimensions);

		/// <summary>
		/// Draw a string onto the screen
		/// </summary>
		/// <param name="text">The text to draw</param>
		/// <param name="position">The position of the string</param>
		/// <param name="scale">The scale of the string</param>
		/// <param name="color">The color of the string</param>
		void DrawString(const std::string& text, glm::vec2& position, glm::vec2& scale = glm::vec2(1.0f), glm::vec4& color = glm::vec4(1.0f));
	private:
		PlayerScript* m_PlayerScript = nullptr;

		Ref<Font> m_Font;
	};
}