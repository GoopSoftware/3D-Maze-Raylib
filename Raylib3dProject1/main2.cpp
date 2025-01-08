#include "raylib.h"
#include "raymath.h"

#include "Player.h"
#include "Bow.h"

#include <iostream>
#include <cmath>
#include <cstdlib>





/*
ToDo

1. Find 3d asset for bow
2. Find asset for arrow
3. Apply assets to screen based on camera pos
4. apply trackArrow values to shooting the arrow
5. apply animation to bow
6. Add a target
7. move target around
8. GUI

*/
int main() {

	const int screenWidth = 800;
	const int screenHeight = 600;
	bool debugEnabled = false;

	
	Player player;
	Bow bow;

	InitWindow(screenWidth, screenHeight, "3D Fun");

	
	Camera3D camera = { 0 };
	camera.position = { 1, 4, 1 };
	camera.target = { 4.0f, 4.0f, 3.0f };
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;
	// To prevent the y coordinate from being effected by movement
	
	

	float mouseSensitivity = 0.003f;
	float yaw = 0.0f;
	float pitch = 0.0f;
	float moveSpeed = 0.15;
	float timeMouseHeld = 0.0f;
	const float maxTimeHeld = 2.f;


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

	bool trackArrowBool = false;



	DisableCursor();

	SetTargetFPS(60);

	while (!WindowShouldClose()) {




		Vector2 mouseDelta = GetMouseDelta();
		yaw -= mouseDelta.x * mouseSensitivity;
		pitch -= mouseDelta.y * mouseSensitivity;

		float pitchAngle = pitch / 1.4708 * 90;
		if (pitchAngle <= -45) pitchAngle = -45;
		bow.angleDegrees = pitchAngle;

		// Prevents looking too far down or up flipping
		if (pitch > PI / 2.0f - 0.1f) pitch = PI / 2.0f - 0.1f;
		if (pitch < -PI / 2.0f + 0.1f) pitch = -PI / 2.0f + 0.1f;

		
		if (yaw < -6.285f) yaw = 0.f;
		if (yaw > 6.285f) yaw = 0.f;


		// Calculates the direction camera is facing
		float yawAngle = yaw / 6.285 * 360;
		float yawRadians = yawAngle * (PI / 180.f);
		
		float arrowDistance = 2.0f;
		float arrowDirX = sinf(yaw);
		float arrowDirZ = cosf(yaw);
		float arrowDirY = sinf(pitch);

		
		Vector3 arrowPosition = { 
				(camera.position.x + arrowDirX * arrowDistance),
				camera.position.y + arrowDirY * arrowDistance,
				(camera.position.z + arrowDirZ * arrowDistance)
		};
		

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



		Ray ray = { camera.position, {0.0f, -1.0f, 0.0f} };
		Vector3 endPoint = Vector3Add(ray.position, Vector3Scale(ray.direction, 10.0f));
		rayDistY = (groundY - ray.position.y) / ray.direction.y;
		/* ------------------ Camera Debug ----------------------
		std::cout << "endPoint: " << endPoint.y << std::endl;
		std::cout << "Camera PosY: " << camera.position.y << std::endl;
		std::cout << "RayDistY: " << rayDistY << std::endl;
		std::cout << "isOnGround: " << isOnGround << std::endl;
		std::cout << "Jumping: " << jumping << std::endl;
		//std::cout << "Velocity: " << velocity << std::endl;
		std::cout << "-----------------------" << std::endl;
		*/


		// Press space to activate bool jump
		if (IsKeyPressed(KEY_SPACE) && isOnGround && !jumping) {
			velocity = jumpForce;
			jumping = true;
			isOnGround = false;
			//std::cout << "*****************************Jump Pressed***************************" << std::endl;
		}

		if (rayDistY < 0) {
			//std::cout << "*****************************Landed***************************" << std::endl;
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

		if (trackArrowBool) {
			bow.trackArrow(yaw);
			if (bow.arrowDest.y <= 0) {
				bow.arrowDest = { 0.0f, 0.0f };
				bow.time = 0.0f;
				trackArrowBool = false;
			}
		}
		

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			timeMouseHeld += GetFrameTime();
			bow.displacement = (bow.maxDisplacement + 1.f) * (timeMouseHeld / maxTimeHeld);
			if (bow.displacement > bow.maxDisplacement) bow.displacement = bow.maxDisplacement;
			bow.arrowVelocity();
			bow.bowElasticPotential();
			bow.stringForceRequired();
			bow.calculateRange();
			bow.calculateTotalDistance();
		}

		else {
			timeMouseHeld = 0.0f;
			bow.displacement = 0.0f;
		}

	
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			std::cout << "BOW FIRED" << std::endl;
		
			trackArrowBool = true;
			bow.printResults();
		}
	

		if (IsKeyPressed(KEY_P)) {
			debugEnabled = !debugEnabled;
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
		arrowPosition.x += bow.arrowDest.x / 2;
		arrowPosition.y += bow.arrowDest.y / 2;
		arrowPosition.z += bow.arrowDest.z / 2;

		BeginDrawing();
		ClearBackground(RAYWHITE);
		BeginMode3D(camera);


		DrawLine3D(ray.position, endPoint, RED);
		DrawCube(arrowPosition, 0.2f, 0.2f, 0.2f, RED);
		DrawGrid(100, 10);



		EndMode3D();

	
		if (debugEnabled) {



			DrawText(TextFormat("Debug Enabled"), screenWidth / 2 - 25, 10, 20, BLACK);

			DrawText(TextFormat("Displacement: %.2f", bow.displacement), 10, 10, 20, BLACK);
			DrawText(TextFormat("Angle: %.2f", bow.angleDegrees), 10, 30, 20, BLACK);
			DrawText(TextFormat("Direction: %.2f", yaw), 10, 50, 20, BLACK);
			DrawText(TextFormat("Bow Elastic Potential: %.2f", bow.elasticPotential), 10, 70, 20, BLACK);
			DrawText(TextFormat("Arrow Velocity: %.2f", bow.velocity), 10, 90, 20, BLACK);
			DrawText(TextFormat("Force Required: %.2f", bow.forceRequired), 10, 110, 20, BLACK);
			DrawText(TextFormat("Distance Arrow Traveled: %.2f", bow.arrowDistance), 10, 130, 20, BLACK);


			DrawText(TextFormat("Time Step: %.2f seconds", bow.time), 10, 150, 20, BLACK);
			DrawText(TextFormat("Arrow X Position: %.2f meters", bow.arrowDest.x), 10, 170, 20, BLACK);
			DrawText(TextFormat("Arrow Y Position: %.2f meters", bow.arrowDest.y), 10, 190, 20, BLACK);
			DrawText(TextFormat("Arrow Distance: %.2f meters", bow.arrowDistance), 10, 210, 20, BLACK);

			DrawText(TextFormat("X: %.2f", camera.position.x), screenWidth - 100, 10, 20, BLACK);
			DrawText(TextFormat("Y: %.2f", camera.position.y), screenWidth - 100, 30, 20, BLACK);
			DrawText(TextFormat("Z: %.2f", camera.position.z), screenWidth - 100, 50, 20, BLACK);

		}
	

		/*
				std::cout << std::fixed << std::setprecision(2);
				std::cout << "Time (s): " << time
					<< "\tDistance (m): " << arrowDest.x
					<< "\tHeight (m): " << arrowDest.y << "\n";
		*/


		EndDrawing();

	}

	CloseWindow();

	return 0;
}