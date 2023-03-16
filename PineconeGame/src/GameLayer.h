#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
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

		Ref<Scene> GetScene() { return m_ActiveScene; }

		static GameLayer& Get() { return *s_Instance; }
	private:
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		Ref<Scene> m_ActiveScene;

		std::vector<GameObject> m_GameObjectsToDestroy;

		GameObject m_Camera;
		GameObject m_Player;
		GameObject m_AsteroidSpawner;
	private:
		static GameLayer* s_Instance;
	};
}