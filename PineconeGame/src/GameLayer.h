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
	private:
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		Ref<Scene> m_ActiveScene;

		GameObject m_Camera;
		GameObject m_Player;
		GameObject m_AsteroidSpawner;
	};
}