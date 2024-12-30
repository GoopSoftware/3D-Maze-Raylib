#include "maze.h"
#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <cstdlib>
#include <iostream>



Maze::Maze() {
	maze.resize(mazeWidth, std::vector<int>(mazeHeight, 1));  // Fill with walls (1)

}

Maze::~Maze() {

}

void Maze::initMaze() {

	for (int x = 0; x < mazeWidth; x++) {
		for (int y = 0; y < mazeHeight; y++) {
			maze[x][y] = 1;
		}
	}
}


void Maze::generateMaze(int x, int y) {
	maze[x][y] = 0;

	int dirOrder[4] = { 0, 1, 2, 3 };
	for (int i = 0; i < 4; i++) {
		int j = rand() % 4;
		std::swap(dirOrder[i], dirOrder[j]);
	}

	for (int i = 0; i < 4; i++) {
		int nx = x + directions[dirOrder[i]][0] * 2;
		int ny = y + directions[dirOrder[i]][1] * 2;

		if (nx >= 0 && nx < mazeWidth && ny >= 0 && ny < mazeHeight && maze[nx][ny] == 1) {
			maze[x + directions[dirOrder[i]][0]][y + directions[dirOrder[i]][1]] = 0;
			generateMaze(nx, ny);
		}
	}
}

void Maze::drawMaze3D() {
	for (int x = 0; x < mazeWidth; x++) {
		for (int y = 0; y < mazeHeight; y++) {
			if (maze[x][y] == 1) {
				Vector3 position = { static_cast<float>(x), 0.5f, static_cast<float>(y) };
				DrawCube(position, 1.0f, 1.0f, 1.0f, DARKGRAY);
			}
		}
	}
}


bool Maze::checkCollisionWithWall(Vector3 newPosition) {
	int gridX = static_cast<int>(std::floor(newPosition.x + 0.5f));
	int gridZ = static_cast<int>(std::floor(newPosition.z + 0.5f));

	if (gridX < 0 || gridX >= mazeWidth || gridZ < 0 || gridZ >= mazeHeight) {
		return true;
	}
	//std::cout << "Checking collision at: " << gridX << " " << gridZ << std::endl;

	return maze[gridX][gridZ] == 1;
}

Vector3 Maze::findValidSpawnPosition() {
	Vector3 spawnPos;
	bool valid = false;
	int attempts = 0;
	const int maxAttempts = 1000; // Limit the number of attempts to find a valid position

	while (!valid && attempts < maxAttempts) {
		int x = rand() % mazeWidth;  // Random x position
		int z = rand() % mazeHeight; // Random z position

		// Check if the position is open (value 0) and not a wall
		if (maze[x][z] == 0) {
			spawnPos = { static_cast<float>(x), 0.5f, static_cast<float>(z) }; // Set spawn position
			valid = true; // Valid spawn position found
		}

		attempts++; // Increase attempts counter
	}

	if (!valid) {
		std::cerr << "Failed to find a valid spawn position after " << maxAttempts << " attempts." << std::endl;
		spawnPos = { 1.0f, 0.5f, 1.0f };  // Default fallback spawn position
	}

	return spawnPos;
}