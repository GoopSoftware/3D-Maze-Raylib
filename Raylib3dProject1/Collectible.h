#pragma once

#include <raylib.h>


class Collectible
{

public:
	Collectible(Vector3 position, const char* modelPath);
	~Collectible();

	void draw(Vector3 playerPosition);
	bool checkCollision(Vector3 playerPosition);
	
	Vector3 position;
	bool collected;
	Model model;
	Texture2D texture;


private:


};