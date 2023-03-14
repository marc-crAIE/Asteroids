#include <Pinecone.h>
#include <Pinecone/Core/EntryPoint.h>

#include "GameLayer.h"

using namespace Pinecone;

namespace PineconeGame
{
	class PineconeApp : public Application
	{
	public:
		PineconeApp()
			: Application()
		{
			srand(time(NULL));

			PushLayer(new AsteroidsGame::GameLayer());
		}
	};
}

Application* Pinecone::CreateApplication()
{
	return new PineconeGame::PineconeApp();
}