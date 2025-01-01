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
	// To prevent the y coordinate from being effected by movement
	Vector3 originalCameraPosition = camera.position;
	Vector3 originalCameraTarget = camera.target;


	float mouseSensitivity = 0.003f;
	float yaw = 0.0f;
	float pitch = 0.0f;
	float moveSpeed = 0.15;

	// physics variables
	const float groundHeight = 4.0f;
	float velocity = 0.0f;
	const float jumpForce = 2.f;
	const float gravity = 0.1f;
	bool isOnGround = true;
	float groundY = 4.0f;
	//float distanceToGround = 0.0f;
	float rayDistY{};
	bool jumping = false;

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


		// if player goes below 9y from a jump set player to 4 and set y velocity to 0
		// player isOnGround
		// allow pressing space key to jump
		// after space key player is not on ground
		// velocity = jumpforce
		// add Velocitry to target and pos y to simulate jump
		// breaking the if statement to the else statement where we subtract velocity by gravity 

		
		Ray ray = { camera.position, {0.0f, -1.0f, 0.0f} };
		Vector3 endPoint = Vector3Add(ray.position, Vector3Scale(ray.direction, 10.0f));
		rayDistY = (groundY - ray.position.y) / ray.direction.y;
		std::cout << "endPoint: " << endPoint.y << std::endl;
		std::cout << "Camera PosY: " << camera.position.y << std::endl;
		std::cout << "RayDistY: " << rayDistY << std::endl;
		std::cout << "isOnGround: " << isOnGround << std::endl;
		std::cout << "Jumping: " << jumping << std::endl;
		//std::cout << "Velocity: " << velocity << std::endl;
		std::cout << "-----------------------" << std::endl;


		// Press space to activate bool jump
		if (IsKeyPressed(KEY_SPACE) && isOnGround && !jumping) {
			velocity = jumpForce;
			jumping = true;
			isOnGround = false;
			std::cout << "*****************************Jump Pressed***************************" << std::endl;
		}

		if (rayDistY < 0) {
			std::cout << "*****************************Landed***************************" << std::endl;
			isOnGround = true;
			jumping = false;
			
		}

		// Applies the jump logic
		// Applies gravity to velocity to bring the camera down
		if (!isOnGround && jumping) {
			camera.target.y += velocity;
			camera.position.y += velocity;
			velocity -= gravity;
		}

		// Normalizes the Y coordinate
		if (isOnGround && !jumping) {
			camera.position.y = groundHeight;
			velocity = 0.0f;
			jumping = false;
		}


		if (IsKeyDown(KEY_LEFT_SHIFT)) {
			moveSpeed = 0.5f;
		}
		else {
			moveSpeed = 0.15f;
		}
	
		if (IsKeyDown(KEY_W)) {
			camera.position = Vector3Add(camera.position, Vector3Scale(forward, moveSpeed));
			camera.target = Vector3Add(camera.target, Vector3Scale(forward, moveSpeed));
			if (!jumping) {
				camera.position.y = originalCameraPosition.y;
			}
		}
		if (IsKeyDown(KEY_S)) {
			camera.position = Vector3Subtract(camera.position, Vector3Scale(forward, moveSpeed));
			camera.target = Vector3Subtract(camera.target, Vector3Scale(forward, moveSpeed));
			if (!jumping) {
				camera.position.y = originalCameraPosition.y;
			}
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

		DrawLine3D(ray.position, endPoint, RED);

		DrawGrid(100, 10);



		EndMode3D();
		EndDrawing();

	}

	CloseWindow();

	return 0;
}