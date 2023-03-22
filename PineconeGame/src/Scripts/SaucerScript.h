#pragma once

#include <Pinecone.h>

using namespace Pinecone;

namespace AsteroidsGame
{
	class SaucerScript : public ScriptableGameObject
	{
	public:
		enum Type { Small = 0, Large };
	public:
		/// <summary>
		/// SaucerScript constructor
		/// </summary>
		/// <param name="type">The size of the saucer (See the SaucerScript::Type enum)</param>
		SaucerScript(Type type);

		/// <summary>
		/// When the SaucerScript is created
		/// </summary>
		void OnCreate() override;
		/// <summary>
		/// When the SaucerScript is destroy
		/// </summary>
		void OnDestroy() override;
		/// <summary>
		/// Update the position of the asteroid
		/// </summary>
		/// <param name="ts">The amount of time passed (in seconds)</param>
		void OnUpdate(Timestep ts) override;

		/// <summary>
		/// Damage the saucer (currently just destroys it) and award a score based on its type to
		/// the player if required
		/// </summary>
		/// <param name="awardScore">If the player should be awarded a score</param>
		void Damage(bool awardScore = true);
	private:
		/// <summary>
		/// Shoot a bullet. 
		/// If the type is small then it will shoot accurately. 
		/// If it is large then shoot in a random direction
		/// </summary>
		void Shoot();
		/// <summary>
		/// Move the saucer in a direction and change that direction after a certain interval
		/// </summary>
		/// <param name="ts">The amount of time passed (in seconds)</param>
		void Wander(Timestep ts);

		/// <summary>
		/// Moves the saucer back onto the screen on the opposite side (left/right top/bottom)
		/// </summary>
		void MoveOntoScreen();
	private:
		Type m_Type;
		glm::vec3 m_Velocity = { 0.0f, 0.0f, 0.0f };
		const float m_Speed = 3.0f;
		const float m_SmallSaucerSize = 2.0f;
		const float m_LargeSaucerSize = 3.0f;

		float m_ShootTime = 0.0f;
		const float m_MaxShootTime = 2.5f;

		const float m_MinChangeDirectionTime = 3.0f;
		const float m_MaxChangeDirectionTime = 8.0f;
		float m_ChangeDirectionElapsed = 0.0f;
		float m_ChangeDirectionTime = m_MaxChangeDirectionTime;
	};
}