#include "Resources.h"

namespace AsteroidsGame
{
	Ref<Texture2D> Resources::s_Asteroid1;
	Ref<Texture2D> Resources::s_Asteroid2;
	Ref<Texture2D> Resources::s_Asteroid3;

	Ref<Texture2D> Resources::s_Saucer;

	Ref<Texture2D> Resources::s_PlayerLife;

	void Resources::Init()
	{
		s_Asteroid1 = Texture2D::Create("assets/textures/Asteroid1.png");
		s_Asteroid2 = Texture2D::Create("assets/textures/Asteroid2.png");
		s_Asteroid3 = Texture2D::Create("assets/textures/Asteroid3.png");

		s_Saucer = Texture2D::Create("assets/textures/Saucer.png");

		s_PlayerLife = Texture2D::Create("assets/textures/PlayerLife.png");
	}

	Ref<Texture2D> Resources::GetAsteroidTexture()
	{
		int selectedTexture = rand() % 3;
		switch (selectedTexture)
		{
		case 0:
			return s_Asteroid1;
		case 1:
			return s_Asteroid2;
		case 2:
			return s_Asteroid3;
		}

		// Shouldn't happen but we will just return s_Asteroid1 by default
		return s_Asteroid1;
	}
}