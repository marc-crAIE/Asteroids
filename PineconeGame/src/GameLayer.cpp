#include "GameLayer.h"

#include "Scripts/PlayerScript.h"
#include "Scripts/AsteroidScript.h"
#include "Scripts/AsteroidSpawnerScript.h"
#include "Scripts/ParticleSpawnerScript.h"

#include <random>

namespace AsteroidsGame
{
	// Initialize the GameLayer instance as null (only set it if we create it)
	GameLayer* GameLayer::s_Instance = nullptr;
	// Initialize the Game Object Names, this is used to specify what Game Object tags to target when resetting
	// the game. Modify accordingly when new object tags that need to be deleted upon reset are made
	std::string GameLayer::s_GameObjectNames[] = { "Bullet", "EnemyBullet", "Asteroid", "Saucer" };

	void GameLayer::OnAttach()
	{
		// Set the clear color for the background of the game
		RenderCommand::SetClearColor({ 0.05f, 0.05f, 0.05f, 1.0f });

		// Set the GameLayer instance, as there can only be one. Assert if it is not null
		PC_ASSERT(!s_Instance, "The Game Layer already exists!");
		s_Instance = this;

		// Create the active scene
		m_ActiveScene = CreateRef<Scene>();

		// Create the Camera Game Object inside the scene
		m_Camera = m_ActiveScene->CreateGameObject("Camera");
		// Add a Camera component and store a reference to it
		auto& cc = m_Camera.AddComponent<CameraComponent>();
		// Set the viewport size as it won't be updated unless you resize the screen (see GameLayer::OnWindowResized)
		cc.Camera.SetViewportSize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		// Set the orthographic projection specifications
		cc.Camera.SetOrthographic(30.0f, -1.0f, 1.0f);

		// Create the Player game object
		m_Player = m_ActiveScene->CreateGameObject("Player");
		// Add the sprite component
		m_Player.AddComponent<SpriteComponent>();
		// Add the Native Script component and bind it to the PlayerScript
		m_Player.AddComponent<NativeScriptComponent>().Bind<PlayerScript>();

		// Create the Asteroid Spawner game object
		m_AsteroidSpawner = m_ActiveScene->CreateGameObject("AsteroidSpawner");
		// Add the Native Script component and bind it to the AsteroidSpawnerScript
		m_AsteroidSpawner.AddComponent<NativeScriptComponent>().Bind<AsteroidSpawnerScript>();

		// Create the Particle Spawner game object
		m_ParticleSpawner = m_ActiveScene->CreateGameObject("ParticleSpawner");
		// Add the Native Script component and bind it to the ParticleSpawnerScript
		m_ParticleSpawner.AddComponent<NativeScriptComponent>().Bind<ParticleSpawnerScript>();
	}

	void GameLayer::OnDetach()
	{

	}

	void GameLayer::OnUpdate(Timestep ts)
	{
		// Reset the 2D Renderer stats (not neccessary)
		Renderer2D::ResetStats();
		// Clear the buffer before drawing to the screen
		RenderCommand::Clear();

		// Check if the game state is in a playing state
		if (m_GameState == GameState::Playing)
		{
			// Update the active scene
			m_ActiveScene->OnUpdate(ts);

			// If there are any Game Objects to be destroyed after updating our scripts. Destroy all of them
			for (auto gameObject : m_GameObjectsToDestroy)
			{
				// Destroy the Game Object in the active scene
				m_ActiveScene->DestroyGameObject(gameObject);
			}
			// Clear the vector as we cannot destroy Game Objects that no longer exist in the scene
			m_GameObjectsToDestroy.clear();

			// Increase the elapsed time since a large saucer spawn
			m_LargeSaucerSpawnTime += ts;

			// Check if the amount of time since we last spawned in a large saucer has met or exceeded the max time
			// between large saucer spawns
			if (m_LargeSaucerSpawnTime >= m_MaxLargeSaucerSpawnTime)
			{
				// Spawn a large saucer
				SpawnSaucer(SaucerScript::Large);
				// Reset the elapsed time
				m_LargeSaucerSpawnTime = 0.0f;
			}
		}
	}

	void GameLayer::OnEvent(Event& e)
	{
		// Create the event dispatcher
		EventDispatcher dispatcher(e);
		// Dispatch the specified events to their appropriate functions
		dispatcher.Dispatch<WindowResizeEvent>(PC_BIND_EVENT_FN(GameLayer::OnWindowResized));
		dispatcher.Dispatch<KeyReleasedEvent>(PC_BIND_EVENT_FN(GameLayer::OnKeyReleased));
	}

	void GameLayer::DestroyGameObject(GameObject gameObject, bool spawnParticles)
	{
		// Add the Game Object to the list of Game Objects to be annihilated
		m_GameObjectsToDestroy.push_back(gameObject);

		// Check if we want to spawn particles.
		// All game objects should have a transform component by default but check for it anyway
		if (spawnParticles && gameObject.HasComponent<TransformComponent>())
		{
			// Get the transform component of the Game Object
			auto& transform = gameObject.GetComponent<TransformComponent>();
			// Emit 8 particles at the Game Object's location
			// TODO: Make 8 a const variable somewhere to be set more visibly somewhere else rather than just
			//		 be hidden inside of here
			((ParticleSpawnerScript*)m_ParticleSpawner.GetComponent<NativeScriptComponent>().Instance)->Emit(transform.Translation, 8);
		}
	}

	void GameLayer::IncreaseScore(int inc)
	{
		// Increase the score and the level score
		m_Score += inc;
		// The level score is used to keep track of how far the player has progressed through the level
		m_LevelScore += inc;

		// Get the player script
		PlayerScript* playerScript = (PlayerScript*)m_Player.GetComponent<NativeScriptComponent>().Instance;

		// Check if the level score has met or exceeded the score to move to the next level
		if (m_LevelScore >= m_NextLevelScore)
		{
			// Add a life to the player
			playerScript->AddLife();
			// But also lets be evil and spawn a super accurately shooting small saucer :D
			SpawnSaucer(SaucerScript::Small);
			// Reset the level score but factor in any score points that went past the next level
			m_LevelScore = m_LevelScore - m_NextLevelScore;
		}
	}

	void GameLayer::SetState(GameState state)
	{
		// If the game state requested is GameOver, make sure the players score is not a new highscore
		if (state == GameState::GameOver && m_Score > 0)
		{
			// Get the highscores
			auto scores = m_UserScores.GetScores();
			// Check to make sure there has been a highscore submitted before
			if (scores.size() > 0)
			{
				// Check if the previous highscore is less than the current score
				if (scores[0].Score < m_Score)
				{
					// Submit a new highscore
					m_GameState = GameState::SubmitHighscore;
					return;
				}
			}
			else
			{
				// Otherwise we will submit the first one
				m_GameState = GameState::SubmitHighscore;
				return;
			}
		}

		// Set the game state
		m_GameState = state;
	}

	void GameLayer::SpawnSaucer(SaucerScript::Type type)
	{
		// Grab the camera component
		auto& camera = m_Camera.GetComponent<CameraComponent>();
		
		// TODO: See the comment inside UILayer::OnUpdate to know how this can be improved
		float orthoSize = camera.Camera.GetOrthographicSize();
		float aspectRatio = camera.Camera.GetAspectRatio();

		float orthoLeft = -orthoSize * aspectRatio * 0.5f;
		float orthoRight = orthoSize * aspectRatio * 0.5f;
		float orthoBottom = -orthoSize * 0.5f;
		float orthoTop = orthoSize * 0.5f;

		// Pick whether the saucer should spawn on the left or right side of the screen
		float xSpawnPos = rand() % 2 == 0 ? orthoLeft : orthoRight;
		// Pick a random y location
		// TODO: Prevent the saucer from spawning too high or low as sometimes it can be hard to see 
		//		 the small saucer (that only moves left and right) when only a bit of it is showing
		float ySpawnPos = rand() % (int)(orthoTop - orthoBottom + 1) + orthoBottom;

		// Create a Saucer
 		GameObject saucer = GetScene()->CreateGameObject("Saucer");
		// Set its position at the spawn x and y positions
		saucer.GetComponent<TransformComponent>().Translation = glm::vec3(xSpawnPos, ySpawnPos, 0.0f);
		// Add the Native Script component and bind the SaucerScript taking the type as an argument for the constructor
		saucer.AddComponent<NativeScriptComponent>().Bind<SaucerScript>(type);
	}

	void GameLayer::ResetGame()
	{
		// Reset the values to default
		m_Score = 0;
		m_LevelScore = 0;
		m_LargeSaucerSpawnTime = 0.0f;

		// Make sure that the Game Objects to be destroyed is empty
		m_GameObjectsToDestroy.clear();

		// Loop through each tag we want to search our Game Objects for
		for (std::string tag : s_GameObjectNames)
		{
			// Get a list of Game Objects of the tag to search
			auto gameObjects = GetScene()->GetGameObjectsByTag(tag);
			// Destroy every Game Object from the scene in that list
			for (auto gameObject : gameObjects)
			{
				m_ActiveScene->DestroyGameObject(gameObject);
			}
		}

		// Call the Reset functions inside of the PlayerScript and AsteroidSpawnerScript classes
		((PlayerScript*)m_Player.GetComponent<NativeScriptComponent>().Instance)->Reset();
		((AsteroidSpawnerScript*)m_AsteroidSpawner.GetComponent<NativeScriptComponent>().Instance)->Reset();
	}

	bool GameLayer::OnKeyReleased(KeyReleasedEvent& e)
	{
		if (m_GameState == GameState::MainMenu)
		{
			// If the user presses the space key while in the main menu. Start playing the game
			if (e.GetKeyCode() == Key::Space)
			{
				SetState(GameState::Playing);
				return true;
			}
		}
		else if (m_GameState == GameState::SubmitHighscore)
		{
			// If the enter key is pressed, submit the score
			if (e.GetKeyCode() == Key::Enter)
			{
				// Make sure the player has entered a name, if so submit the score
				if (m_PlayerName.size() > 0)
				{
					m_UserScores.AddScore(m_PlayerName, m_Score);
					m_UserScores.Save();
				}
				// Set the game to be game over
				SetState(GameState::GameOver);
			}
			else if (e.GetKeyCode() == Key::Backspace)
			{
				// Remove the last character from te player name
				if (m_PlayerName.size() > 0)
					m_PlayerName.pop_back();
			}
			else if (m_PlayerName.size() < m_MaxPlayerNameSize)
			{
				// Make sure that the user has entered a valid character
				// These are characters for A-Z and 0-9
				if (e.GetKeyCode() >= Key::A && e.GetKeyCode() <= Key::Z)
				{
					m_PlayerName += e.GetKeyCode();
				}
				else if (e.GetKeyCode() >= Key::D0 && e.GetKeyCode() <= Key::D9)
				{
					m_PlayerName += std::to_string(e.GetKeyCode() - Key::D0);
				}
			}
		}
		else if (m_GameState == GameState::GameOver)
		{
			// If the user presses the space key while in the game over screen. Replay the game
			if (e.GetKeyCode() == Key::Space)
			{
				SetState(GameState::Playing);
				ResetGame();
				return true;
			}
			// If the user presses the escape key while in the game over screen. Go to the main menu
			else if (e.GetKeyCode() == Key::Escape)
			{
				SetState(GameState::MainMenu);
				// Prepare the game to be replayed from the main menu
				ResetGame();
				return true;
			}
		}
		else
		{
			// If the user presses the escape key while playing or in the paused menu. Play or pause the game
			if (e.GetKeyCode() == Key::Escape)
			{
				SetState(m_GameState == GameState::Playing ? GameState::Paused : GameState::Playing);
				return true;
			}
		}
		return false;
	}

	bool GameLayer::OnWindowResized(WindowResizeEvent& e)
	{
		// Change the viewport for the active scene which sets the viewport for every Game Object that has
		// a camera component attached to it
		m_ActiveScene->OnViewportResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}