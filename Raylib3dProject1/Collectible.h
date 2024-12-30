#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include <raylib.h>


class Collectible
{

public:
	Collectible(Vector3 position);
	~Collectible();

	void draw();
	bool checkCollision(Vector3 playerPosition);
	
	Vector3 position;
	bool collected;

private:


};

#endif // !1
