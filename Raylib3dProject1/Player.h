#pragma once

#include "raylib.h"
#include "raymath.h"


class Player
{

public:
	Player();
	~Player();


	void drawPlayer();


	Vector3 position = { 1.f, 4.f, 1.f };
	Vector3 target = { 4.0f, 4.0f, 3.0f };


private:


	const float groundHeight = 4.0f;
	

};


