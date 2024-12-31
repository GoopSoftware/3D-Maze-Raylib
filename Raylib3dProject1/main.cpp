#include "raylib.h"
#include "raymath.h"
#include "maze.h"
#include "Collectible.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>


int main() {


	Maze maze;


	const int screenWidth = 800;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "3D Maze");

	Camera3D camera = { 0 };
	camera.position = maze.findValidSpawnPosition();
	camera.target = { 4.0f, 2.0f, 3.0f };
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;



	float moveSpeed = 0.06f;
	float mouseSensitivity = 0.003f;

	float yaw = 0.0f;
	float pitch = 0.0f;

	DisableCursor();

	srand(time(0));

	maze.initMaze();
	maze.generateMaze(1, 1);
	maze.placeCollectible();

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		
		camera.position.y = .6f;

		Vector2 mouseDelta = GetMouseDelta();
		yaw -= mouseDelta.x * mouseSensitivity;
		pitch -= mouseDelta.y * mouseSensitivity;

		if (pitch > PI / 2.0f - 0.1f) pitch = PI / 2.0f - 0.1f;
		if (pitch < -PI / 2.0f + 0.1f) pitch = -PI / 2.0f + 0.1f;

		Vector3 forward = {
				cosf(pitch) * sinf(yaw),
				sinf(pitch),
				cosf(pitch) * cosf(yaw)
			};

		forward = Vector3Normalize(forward);

		camera.target = Vector3Add(camera.position, forward);

	
		Vector3 right = Vector3CrossProduct(forward, Vector3Normalize(camera.up));
		right.y = 0.0f;
		right = Vector3Normalize(right);


		if (IsKeyDown(KEY_LEFT_SHIFT)) {
			moveSpeed = 0.01f;
		}
		else {
			moveSpeed = 0.08f;
		}

		if (IsKeyPressed(KEY_R)) {
			maze.placeCollectible();
		}

		if (IsKeyDown(KEY_W)) {
			Vector3 newPos = Vector3Add(camera.position, Vector3Scale(forward, moveSpeed));
			if (!maze.checkCollisionWithWall(newPos)) {
				//std::cout << "No collision detected\n";

				std::cout << camera.position.x << std::endl;
				std::cout << camera.position.z << std::endl;
				camera.position = newPos;
				camera.target = Vector3Add(camera.target, Vector3Scale(forward, moveSpeed));
			}
		}
		if (IsKeyDown(KEY_S)) {
			Vector3 newPos = Vector3Subtract(camera.position, Vector3Scale(forward, moveSpeed));
			if (!maze.checkCollisionWithWall(newPos)) {
				//std::cout << "No collision detected\n";

				std::cout << camera.position.x << std::endl;
				std::cout << camera.position.z << std::endl;
				camera.position = newPos;
				camera.target = Vector3Subtract(camera.target, Vector3Scale(forward, moveSpeed));
			}
			
		}
		if (IsKeyDown(KEY_D)) {
			Vector3 newPos = Vector3Add(camera.position, Vector3Scale(right, moveSpeed));
			if (!maze.checkCollisionWithWall(newPos)) {
				//std::cout << "No collision detected\n";

				std::cout << camera.position.x << std::endl;
				std::cout << camera.position.z << std::endl;
				camera.position = newPos;
				camera.target = Vector3Add(camera.target, Vector3Scale(right, moveSpeed));
			}
			
		}
		if (IsKeyDown(KEY_A)) {
			Vector3 newPos = Vector3Subtract(camera.position, Vector3Scale(right, moveSpeed));
			if (!maze.checkCollisionWithWall(newPos)) {
				//std::cout << "No collision detected\n";

				std::cout << camera.position.x << std::endl;
				std::cout << camera.position.z << std::endl;
				camera.position = newPos;
				camera.target = Vector3Subtract(camera.target, Vector3Scale(right, moveSpeed));
			}
			
		}

	


		BeginDrawing();
		ClearBackground(RAYWHITE);

		BeginMode3D(camera);
		maze.drawMaze3D(camera.position);
		
		DrawPlane({ 7.0f, 0.0f, 7.0f }, { 15.0f, 15.0f }, LIGHTGRAY);
		//DrawGrid(10, 1.0f);


		EndMode3D();
		if (maze.collectible != nullptr && maze.collectible->checkCollision(camera.position)) {
			DrawText("YOU FOUND JOE!!!! Press R to find Joe again!", screenWidth / 2 - 250, screenHeight / 2, 25, RED);
		}

		DrawText("Joe is lost! Find Joe!", 10, 10, 25, RED);

		EndDrawing();

	}

	CloseWindow();

	return 0;
}