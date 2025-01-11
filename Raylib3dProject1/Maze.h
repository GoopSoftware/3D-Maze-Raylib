#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include "Collectible.h"
#include <raylib.h>

class Maze
{

public:
	Maze();
	~Maze();

	void initMaze();
	void generateMaze(int x, int y);
	void drawMaze3D(Vector3 playerPosition);
	bool isWallAtPosition(int x, int y);
	bool checkCollisionWithWall(Vector3 newPosition);
	Vector3 findValidSpawnPosition();
	void placeCollectible();
	Collectible* collectible;

private:

	const int directions[4][2] = {
		{0, 1},
		{1, 0},
		{0, -1},
		{-1, 0},
	};


	const int mazeWidth = 15;
	const int mazeHeight = 15;

	std::vector<std::vector<int>> maze;


};

#endif