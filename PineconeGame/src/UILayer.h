#pragma once

#include <Pinecone.h>

#include "Scripts/PlayerScript.h"

using namespace Pinecone;

namespace AsteroidsGame
{
	class UILayer : public Layer
	{
	public:
		UILayer();
		~UILayer() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;
	private:
		void DrawLivesAndScore(glm::vec4& screenDimensions);
	private:
		PlayerScript* m_PlayerScript = nullptr;

		Ref<Font> m_Font;
	};
}