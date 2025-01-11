#include "Enemy.h"

#include <iostream>


Enemy::Enemy(Vector3 position, const char* modelPath, const char* texturePath){
	this->position = position;
	shot = false;

	model = LoadModel(modelPath);

	if (model.meshCount == 0) {
		TraceLog(LOG_ERROR, "FAILED TO LOAD MODEL: %s", modelPath);
		throw std::runtime_error("Failed to load model");
	}
	texture = LoadTexture(texturePath);

	if (texture.id == 0) {
		TraceLog(LOG_ERROR, "FAILED TO LOAD TEXTURE: %s", texturePath);
		throw std::runtime_error("Failed to load TEXTURE");
	}
	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
}

Enemy::~Enemy() {

	UnloadModel(model);
	UnloadTexture(texture);
}

void Enemy::draw(Vector3 playerPos) {
	
	Vector3 direction = Vector3Subtract(playerPos, position);

	direction = Vector3Normalize(direction);
	
	float angle = atan2f(direction.x, direction.z);

	angle -= PI / 2.0f;

	Matrix rotation = MatrixRotateY(angle);
	
	DrawModelEx(model, position, { 0.0f, 1.0f, 0.0f }, angle * RAD2DEG, {scale, scale, scale }, WHITE);

	DrawSphereWires(position, boundingSphereRadius, 16, 16, RED);


}


void Enemy::chasePlayer(Vector3 playerPos, float deltaTime) {
	
	chaseDir = Vector3Subtract(position, playerPos);

	Vector3 normChaseDir = Vector3Normalize(chaseDir);
	
	distanceToPlayer = Vector3Distance(position, playerPos);
	//chaseSpeed += .01f;
	
	std::cout << "DistToPlayer: " << distanceToPlayer << std::endl;

	if (distanceToPlayer > minDistance) {
		position.x -= normChaseDir.x * chaseSpeed * deltaTime;
		position.y -= normChaseDir.y * chaseSpeed * deltaTime;
		position.z -= normChaseDir.z * chaseSpeed * deltaTime;
	}

	/*
	if (CheckCollisionSpheres(enemy.position, enemy.boundingSphereRadius, player.position, 4)) {
		Vector3 prevEnemyPos = enemy.position;
		enemy.position = prevEnemyPos;

	}
	*/


}