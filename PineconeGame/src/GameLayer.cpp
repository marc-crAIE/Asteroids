#include "GameLayer.h"

#include "Scripts/PlayerScript.h"
#include "Scripts/AsteroidSpawnerScript.h"

namespace AsteroidsGame
{
	void GameLayer::OnAttach()
	{
		RenderCommand::SetClearColor({ 0.05f, 0.05f, 0.05f, 1.0f });

		m_ActiveScene = CreateRef<Scene>();

		m_Camera = m_ActiveScene->CreateGameObject("Camera");
		auto& cc = m_Camera.AddComponent<CameraComponent>();
		cc.Camera.SetViewportSize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		cc.Camera.SetOrthographic(20.0f, -1.0f, 1.0f);

		m_Player = m_ActiveScene->CreateGameObject("Player");
		m_Player.AddComponent<SpriteComponent>();
		m_Player.AddComponent<NativeScriptComponent>().Bind<PlayerScript>();

		m_AsteroidSpawner = m_ActiveScene->CreateGameObject("AsteroidSpawner");
		m_AsteroidSpawner.AddComponent<NativeScriptComponent>().Bind<AsteroidSpawnerScript>();
	}

	void GameLayer::OnDetach()
	{

	}

	void GameLayer::OnUpdate(Timestep ts)
	{
		Renderer2D::ResetStats();
		RenderCommand::Clear();

		m_ActiveScene->OnUpdate(ts);
	}

	void GameLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(PC_BIND_EVENT_FN(GameLayer::OnWindowResized));
	}

	bool GameLayer::OnWindowResized(WindowResizeEvent& e)
	{
		m_ActiveScene->OnViewportResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}