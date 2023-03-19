#pragma once

#include <Pinecone.h>
#include <Pinecone/Events/KeyEvent.h>

#include "Scripts/SaucerScript.h"

using namespace Pinecone;

namespace AsteroidsGame
{
	enum class GameState
	{
		Playing = 0,
		MainMenu,
		GameOver,
		Paused
	};

	class GameLayer : public Layer
	{
	public:
		GameLayer() : Layer("GameLayer") {}
		~GameLayer() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;

		void DestroyGameObject(GameObject gameObject);

		void IncreaseScore(int inc);
		void SetState(GameState state) { m_GameState = state; }

		Ref<Scene> GetScene() const { return m_ActiveScene; }
		GameState GetState() const { return m_GameState; }
		int GetScore() const { return m_Score; }

		static GameLayer& Get() { return *s_Instance; }
	private:
		void SpawnSaucer(SaucerScript::Type type);

		void ResetGame();

		bool OnKeyReleased(KeyReleasedEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		Ref<Scene> m_ActiveScene;
		GameState m_GameState = GameState::MainMenu;

		std::vector<GameObject> m_GameObjectsToDestroy;

		GameObject m_Camera;
		GameObject m_Player;
		GameObject m_AsteroidSpawner;

		int m_Score = 0;
		int m_LevelScore = 0;
		const int m_NextLevelScore = 10000;

		float m_LargeSaucerSpawnTime = 0.0f;
		const float m_MaxLargeSaucerSpawnTime = 120.0f;
	private:
		static GameLayer* s_Instance;
		static std::string s_GameObjectNames[];
	};
}