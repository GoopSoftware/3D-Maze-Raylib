#include "raylib.h"
#include "raymath.h"

#include <iostream>
#include <cmath>
#include <cstdlib>



int main() {

	const int screenWidth = 800;
	const int screenHeight = 600;
	
	
	InitWindow(screenWidth, screenHeight, "3D Fun");

	Camera3D camera = { 0 };
	camera.position = { 1, 4, 1 };
	camera.target = { 4.0f, 4.0f, 3.0f };
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;


	float mouseSensitivity = 0.003f;
	float yaw = 0.0f;
	float pitch = 0.0f;
	float moveSpeed = 0.15;


	// physics variables
	const float groundHeight = 4.0f;
	float velocity = 0.0f;
	const float jumpForce = 5.f;
	const float gravity = 0.1f;
	bool isOnGround = false;
	float timeStep = .016f;

	DisableCursor();

	SetTargetFPS(60);

	while (!WindowShouldClose()) {


		Vector2 mouseDelta = GetMouseDelta();
		yaw -= mouseDelta.x * mouseSensitivity;
		pitch -= mouseDelta.y * mouseSensitivity;

		// Prevents looking too far down or up flipping
		if (pitch > PI / 2.0f - 0.1f) pitch = PI / 2.0f - 0.1f;
		if (pitch < -PI / 2.0f + 0.1f) pitch = -PI / 2.0f + 0.1f;

		Vector3 forward = {
				cosf(pitch) * sinf(yaw),
				sinf(pitch),
				cosf(pitch) * cosf(yaw)
		};

		forward = Vector3Normalize(forward);
		// Moves the Camera
		Vector3 right = Vector3CrossProduct(forward, Vector3Normalize(camera.up));
		right.y = 0.0f;
		right = Vector3Normalize(right);
		camera.target = Vector3Add(camera.position, forward);


		
		std::cout << isOnGround;
		if (camera.position.y <= groundHeight + 5) {
			//camera.target.y = groundHeight;
			camera.position.y = groundHeight;
			velocity = 0.0f;
			isOnGround = true;
			if (IsKeyPressed(KEY_SPACE)) {
				isOnGround = false;
				velocity = jumpForce;
				camera.target.y += velocity;
				camera.position.y += velocity;
			}
		}
		else {
			velocity -= gravity;
		}
		//float nextY = camera.position.y + velocity * timeStep;
		

		if (!isOnGround) {
			camera.target.y += velocity;
			camera.position.y += velocity;
		
		}
		if (isOnGround) {
			camera.position.y = 4.f;
		}
		std::cout << camera.position.y << std::endl;


		if (IsKeyDown(KEY_LEFT_SHIFT)) {
			moveSpeed = 0.5f;
		}
		else {
			moveSpeed = 0.15f;
		}


	
		if (IsKeyDown(KEY_W)) {
			camera.position = Vector3Add(camera.position, Vector3Scale(forward, moveSpeed));
			camera.target = Vector3Add(camera.target, Vector3Scale(forward, moveSpeed));
		}
		if (IsKeyDown(KEY_S)) {
			camera.position = Vector3Subtract(camera.position, Vector3Scale(forward, moveSpeed));
			camera.target = Vector3Subtract(camera.target, Vector3Scale(forward, moveSpeed));
		}
		if (IsKeyDown(KEY_A)) {
			camera.position = Vector3Subtract(camera.position, Vector3Scale(right, moveSpeed));
			camera.target = Vector3Subtract(camera.target, Vector3Scale(right, moveSpeed));
		}
		if (IsKeyDown(KEY_D)) {
			camera.position = Vector3Add(camera.position, Vector3Scale(right, moveSpeed));
			camera.target = Vector3Add(camera.target, Vector3Scale(right, moveSpeed));
		}


	


		BeginDrawing();
		ClearBackground(RAYWHITE);


		BeginMode3D(camera);
		
		DrawGrid(100, 10);

		EndMode3D();
		EndDrawing();

	}

	CloseWindow();

	return 0;
}

