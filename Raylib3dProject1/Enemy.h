#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "raymath.h"


class Enemy
{

public:
	Enemy(Vector3 position, const char* modelPath, const char* texturePath);
	~Enemy();

	void draw(Vector3 playerPos);
	void chasePlayer(Vector3 playerPos, float deltaTime);

	bool shot;
	Vector3 position;
	Model model;
	Texture2D texture;

	float chaseSpeed = 10.f;
	Vector3 chaseDir;



private:


};
#endif