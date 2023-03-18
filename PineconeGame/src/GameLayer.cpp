#include "GameLayer.h"

#include "Scripts/PlayerScript.h"
#include "Scripts/AsteroidScript.h"
#include "Scripts/AsteroidSpawnerScript.h"

#include <random>

namespace AsteroidsGame
{
	GameLayer* GameLayer::s_Instance = nullptr;

	void GameLayer::OnAttach()
	{
		RenderCommand::SetClearColor({ 0.05f, 0.05f, 0.05f, 1.0f });

		PC_ASSERT(!s_Instance, "The Game Layer already exists!");
		s_Instance = this;

		m_ActiveScene = CreateRef<Scene>();

		m_Camera = m_ActiveScene->CreateGameObject("Camera");
		auto& cc = m_Camera.AddComponent<CameraComponent>();
		cc.Camera.SetViewportSize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		cc.Camera.SetOrthographic(30.0f, -1.0f, 1.0f);

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

		for (auto gameObject : m_GameObjectsToDestroy)
		{
			m_ActiveScene->DestroyGameObject(gameObject);
		}
		m_GameObjectsToDestroy.clear();
	}

	void GameLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(PC_BIND_EVENT_FN(GameLayer::OnWindowResized));
	}

	void GameLayer::DestroyGameObject(GameObject gameObject)
	{
		m_GameObjectsToDestroy.push_back(gameObject);
	}

	void GameLayer::IncreaseScore(int inc)
	{
		m_Score += inc;
		m_TenThousandScore += inc;

		PlayerScript* playerScript = (PlayerScript*)m_Player.GetComponent<NativeScriptComponent>().Instance;

		if (m_TenThousandScore >= m_NextLevelScore)
		{
			playerScript->AddLife();
			m_TenThousandScore = m_TenThousandScore - m_NextLevelScore;
			SpawnSaucer(SaucerScript::Small);
		}
	}

	void GameLayer::SpawnSaucer(SaucerScript::Type type)
	{
		auto& camera = GetScene()->GetPrimaryCameraGameObject().GetComponent<CameraComponent>();

		float orthoSize = camera.Camera.GetOrthographicSize();
		float aspectRatio = camera.Camera.GetAspectRatio();

		float orthoLeft = -orthoSize * aspectRatio * 0.5f;
		float orthoRight = orthoSize * aspectRatio * 0.5f;
		float orthoBottom = -orthoSize * 0.5f;
		float orthoTop = orthoSize * 0.5f;

		float xSpawnPos = rand() % 2 == 0 ? orthoLeft : orthoRight;
		float ySpawnPos = rand() % (int)(orthoTop - orthoBottom + 1) + orthoBottom;

		glm::vec3 spawnPos = glm::vec3(xSpawnPos, ySpawnPos, 0.0f);

 		GameObject saucer = GetScene()->CreateGameObject();
		saucer.GetComponent<TransformComponent>().Translation = spawnPos;
		saucer.AddComponent<NativeScriptComponent>().Bind<SaucerScript>(type);
	}

	bool GameLayer::OnWindowResized(WindowResizeEvent& e)
	{
		m_ActiveScene->OnViewportResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}