#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class Physics2D
	{
	public:
		static bool CheckRectCollision(GameObject& a, GameObject& b);
	};
}