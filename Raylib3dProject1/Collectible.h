#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include <raylib.h>


class Collectible
{

public:
	Collectible(Vector3 position, const char* modelPath);
	~Collectible();

	void draw();
	bool checkCollision(Vector3 playerPosition);
	
	Vector3 position;
	bool collected;
	Model model;
	Texture2D texture;

private:


};

#endif // !1
