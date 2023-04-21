#pragma once

#include <Pinecone.h>
#include <Pinecone/Events/KeyEvent.h>

#include "Scripts/SaucerScript.h"
#include "Utils/ParticleSystem.h"

#include "UserScore.h"

using namespace Pinecone;

namespace AsteroidsGame
{
	enum class GameState
	{
		Playing = 0,
		MainMenu,
		SubmitHighscore,
		GameOver,
		Paused
	};

	class GameLayer : public Layer
	{
	public:
		/// <summary>
		/// The GameLayer constructor. Sets the layer name to be "GameLayer"
		/// </summary>
		GameLayer() : Layer("GameLayer"), 
			m_UserScores("user_scores.dat") 
		{}
		/// <summary>
		/// The GameLayer deconstructor
		/// </summary>
		~GameLayer() = default;

		/// <summary>
		/// Called when the GameLayer is attached
		/// </summary>
		void OnAttach() override;
		/// <summary>
		/// Called when the GameLayer is detached
		/// </summary>
		void OnDetach() override;

		/// <summary>
		/// Update the active game scene if playing, destroy game objects, and spawn large saucers
		/// </summary>
		/// <param name="ts">The time passed (in seconds)</param>
		void OnUpdate(Timestep ts) override;
		/// <summary>
		/// Handle our events from the application
		/// </summary>
		/// <param name="e">The event to be handled</param>
		void OnEvent(Event& e) override;

		/// <summary>
		/// Add a game object to be destroyed after the script update loop is complete
		/// </summary>
		/// <param name="gameObject">The game object to be destroyed</param>
		/// <param name="spawnParticles">Whether to spawn particles where the game object was destroyed</param>
		void DestroyGameObject(GameObject gameObject, bool spawnParticles = true);

		/// <summary>
		/// Increase the game score by a specified amount
		/// </summary>
		/// <param name="inc">The amount to increase the score</param>
		void IncreaseScore(int inc);
		/// <summary>
		/// Change the state of the game to a specified game state
		/// </summary>
		/// <param name="state">The game state to change to</param>
		void SetState(GameState state);
		/// <summary>
		/// Get the current game state
		/// </summary>
		/// <returns>The current game state</returns>
		GameState GetState() const { return m_GameState; }

		/// <summary>
		/// Get the current active scene
		/// </summary>
		/// <returns>The active scene</returns>
		Ref<Scene> GetScene() const { return m_ActiveScene; }
		/// <summary>
		/// Get the current game score
		/// </summary>
		/// <returns>The game score</returns>
		int GetScore() const { return m_Score; }
		/// <summary>
		/// Get a list of scores ordered from highest score to lowest
		/// </summary>
		/// <returns>The scores listed from highest to lowest</returns>
		std::vector<UserScore> GetScores() const { return m_UserScores.GetScores(); }

		/// <summary>
		/// Gets the currently entered player name
		/// </summary>
		/// <returns>The player name</returns>
		std::string GetPlayerName() const { return m_PlayerName; }

		/// <summary>
		/// Get the instance of the Gamelayer. GameLayer has a static instance of itself as I also use it
		/// as a state manager and to store the games overall state.
		/// There can only be 1 GameLayer (otherwise an assertion is called, see the OnAttach() function)
		/// </summary>
		/// <returns>The GameLayer</returns>
		static GameLayer& Get() { return *s_Instance; }
	private:
		/// <summary>
		/// Spawn a saucer of a specified type
		/// </summary>
		/// <param name="type">The type of saucer to spawn (see Saucer::Type)</param>
		void SpawnSaucer(SaucerScript::Type type);

		/// <summary>
		/// Reset the game state to default values along with the Player and AsteroidSpawner Game Objects. Clears all game objects with tags specified in the 
		/// s_GameObjectNames variable.
		/// </summary>
		void ResetGame();

		/// <summary>
		/// Handle key released events
		/// </summary>
		/// <param name="e">The key released event</param>
		/// <returns>If the event was handled</returns>
		bool OnKeyReleased(KeyReleasedEvent& e);
		/// <summary>
		/// Handle window resized events
		/// </summary>
		/// <param name="e">The window resized event</param>
		/// <returns>If the event was handled</returns>
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		Ref<Scene> m_ActiveScene;
		GameState m_GameState = GameState::MainMenu;

		std::vector<GameObject> m_GameObjectsToDestroy;

		// These do not need to be pointers as GameObject essentially just stores the entt library's 
		// Entity handle (which is a unsigned int). That entity handle is how we know what game 
		// object to get and add the components to.
		GameObject m_Camera;
		GameObject m_Player;
		GameObject m_AsteroidSpawner;
		GameObject m_ParticleSpawner;

		UserScores m_UserScores;
		std::string m_PlayerName = "";
		const int m_MaxPlayerNameSize = 8;

		int m_Score = 0;
		int m_LevelScore = 0;
		const int m_NextLevelScore = 10000;

		float m_LargeSaucerSpawnTime = 0.0f;
		const float m_MaxLargeSaucerSpawnTime = 30.0f;
	private:
		static GameLayer* s_Instance;
		static std::string s_GameObjectNames[];
	};
}