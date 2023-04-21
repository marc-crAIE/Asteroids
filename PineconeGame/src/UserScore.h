#pragma once

#include <string>
#include <vector>
#include <map>

namespace AsteroidsGame
{
	struct UserScore
	{
		std::string Name;
		uint32_t Score;
	};

	class UserScores
	{
	public:
		UserScores(std::string path);

		/// <summary>
		/// Load the user score file
		/// </summary>
		void Load();
		/// <summary>
		/// Save the user score file
		/// </summary>
		void Save();

		/// <summary>
		/// Add a score to the database with a given player name
		/// </summary>
		/// <param name="name">The player name</param>
		/// <param name="score">The players score</param>
		void AddScore(std::string name, uint32_t score);
		/// <summary>
		/// Get a list of scores ordered from highest score to lowest
		/// </summary>
		/// <returns>The scores listed from highest to lowest</returns>
		std::vector<UserScore> GetScores() const;
	private:
		void Clear();
	private:
		std::string m_FilePath;
		std::map<std::string, uint32_t> m_Scores;
	};
}