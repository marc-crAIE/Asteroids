#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class Resources
	{
	public:
		static void Init();

		static Ref<Texture2D> GetAsteroidTexture();
		static Ref<Texture2D> GetPlayerLifeTexture() { return s_PlayerLife; }
		static Ref<Texture2D> GetSaucerTexture() { return s_Saucer; }
	private:
		static Ref<Texture2D> s_Asteroid1;
		static Ref<Texture2D> s_Asteroid2;
		static Ref<Texture2D> s_Asteroid3;

		static Ref<Texture2D> s_Saucer;

		static Ref<Texture2D> s_PlayerLife;
	};
}