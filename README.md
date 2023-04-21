# PineconeGame

This game is a basic recreation of the classic arcade game from ATARI also named Asteroids released in 1979. The objective of the game is to navigate a spaceship through an asteroid field while avoiding collisions with the asteroids and enemy ships. The game is a high-score based game, where the player's goal is to achieve the highest score possible by destroying asteroids and enemy ships.

## Downloading the repository

Clone the repository with `git clone --recursive https://github.com/marc-crAIE/Asteroids`.

If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.

## Configuring the dependencies

Run the [Setup.bat](/scripts/Setup.bat) file found in scripts folder.
The [Win-GenProjects.bat](/scripts/Win-GenProjects.bat) script file will get executed automatically, which will then generate a Visual Studio solution file for user's usage.

If changes are made or if you want to regenerate project files, re-run the [Win-GenProjects.bat](/scripts/Win-GenProjects.bat) script file found in `scripts` folder.