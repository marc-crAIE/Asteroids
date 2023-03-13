#include <Pinecone.h>
#include <Pinecone/Core/EntryPoint.h>

using namespace Pinecone;

namespace PineconeGame
{
	class PineconeApp : public Application
	{
	public:
		PineconeApp()
			: Application()
		{

		}
	};
}

Application* Pinecone::CreateApplication()
{
	return new PineconeGame::PineconeApp();
}