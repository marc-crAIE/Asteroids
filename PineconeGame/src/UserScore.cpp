#include "UserScore.h"

#include <Pinecone.h>
#include <fstream>

namespace AsteroidsGame
{
	UserScores::UserScores(std::string path)
		: m_FilePath(path)

	{
		Load();
	}

	void UserScores::Load()
	{
		Clear();

		std::ifstream infile(m_FilePath, std::ios::binary);

		if (infile.is_open())
		{
			uint32_t scoresCount = 0;
			infile.read((char*)&scoresCount, sizeof(uint32_t));

			for (int i = 0; i < scoresCount; i++)
			{
				uint32_t nameSize = 0;
				infile.read((char*)&nameSize, sizeof(uint32_t));

				char* name = new char[nameSize + 1];
				infile.read(name, nameSize);
				name[nameSize] = '\0';

				uint32_t score;
				infile.read((char*)&score, sizeof(uint32_t));

				m_Scores[name] = score;

				delete[] name;
			}
		}
		else
		{
			PC_ERROR("Failed to load high score file \"{0}\"", m_FilePath);
		}

		infile.close();
	}

	void UserScores::Save()
	{
		std::ofstream outfile(m_FilePath, std::ios::binary);

		if (outfile.is_open())
		{
			uint32_t scoreCount = m_Scores.size();
			outfile.write((char*)&scoreCount, sizeof(uint32_t));

			for (const auto& [name, score] : m_Scores)
			{
				uint32_t nameSize = name.length();
				outfile.write((char*)&nameSize, sizeof(uint32_t));
				outfile.write(name.c_str(), name.length());
				outfile.write((char*)&score, sizeof(uint32_t));
			}
		}
		else
		{
			PC_ERROR("Failed to write to high score file \"{0}\"", m_FilePath);
		}

		outfile.close();
	}

	void UserScores::AddScore(std::string name, uint32_t score)
	{
		m_Scores[name] = score;
	}

	std::vector<UserScore> UserScores::GetScores() const
	{
		std::vector<UserScore> scores;
		for (const auto& entry : m_Scores)
		{
			UserScore userScore = { entry.first, entry.second };
			scores.push_back(userScore);
		}

		std::sort(scores.begin(), scores.end(), [=](const UserScore& a, const UserScore& b)
			{
				return a.Score > b.Score;
			});
		return scores;
	}

	void UserScores::Clear()
	{
		m_Scores.clear();
	}
}