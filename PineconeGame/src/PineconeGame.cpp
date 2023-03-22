#include <Pinecone.h>
#include <Pinecone/Core/EntryPoint.h>

#include "GameLayer.h"
#include "UILayer.h"
#include "Utils/Resources.h"

using namespace Pinecone;

namespace PineconeGame
{
	class PineconeApp : public Application
	{
	public:
		PineconeApp()
			: Application()
		{
			// Initialize random
			// Note: the rand() function is not always used as I also often use a different randomizer function
			srand(time(NULL));

			// Initialize our resources
			AsteroidsGame::Resources::Init();

			// Push our layers to the application
			PushLayer(new AsteroidsGame::GameLayer());
			PushOverlay(new AsteroidsGame::UILayer());
		}
	};
}

// This function is essential for creating the application that
// the main entry point will use (Pinecone/Core/EntryPoint.h)
Application* Pinecone::CreateApplication()
{
	return new PineconeGame::PineconeApp();
}