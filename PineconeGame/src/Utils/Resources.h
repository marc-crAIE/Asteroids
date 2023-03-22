#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class Resources
	{
	public:
		/// <summary>
		/// Initialize our resources
		/// </summary>
		static void Init();

		/// <summary>
		/// Get an asteroid texture by picking a random one out of the 3
		/// </summary>
		/// <returns>A random asteroid texture</returns>
		static Ref<Texture2D> GetAsteroidTexture();
		/// <summary>
		/// Get the player life texture
		/// </summary>
		/// <returns>Player life texture</returns>
		static Ref<Texture2D> GetPlayerLifeTexture() { return s_PlayerLife; }
		/// <summary>
		/// Get the saucer texture
		/// </summary>
		/// <returns>Saucer texture</returns>
		static Ref<Texture2D> GetSaucerTexture() { return s_Saucer; }
	private:
		static Ref<Texture2D> s_Asteroid1;
		static Ref<Texture2D> s_Asteroid2;
		static Ref<Texture2D> s_Asteroid3;

		static Ref<Texture2D> s_Saucer;

		static Ref<Texture2D> s_PlayerLife;
	};
}