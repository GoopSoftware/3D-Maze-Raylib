#include "raylib.h"
#include "raymath.h"

#include "Player.h"
#include "Bow.h"
#include "Maze.h"
#include "Enemy.h"

#include <iostream>
#include <cmath>
#include <cstdlib>





/*
ToDo

- Find 3d asset for bow
- Find asset for arrow
- Apply assets to screen based on camera pos

- apply animation to bow
- move target around
- GUI


The 3d model will spawn randomly around the map, if an enemy spawns behind the player a red arrow will appear 
on the side of the screen to steer the player. The enemy will head towards the player at increasing speed
the player must shoot the enemy before the enemy hits the player 3 times

*/

void DrawReticle(int screenWidth, int screenHeight, float size) {
	float centerX = screenWidth / 2.0f;
	float centerY = screenHeight / 2.0f;

	DrawLine(centerX - size, centerY, centerX + size, centerY, RED);
	DrawLine(centerX, centerY - size, centerX, centerY + size, RED);
}

int main() {

	const int screenWidth = 1280;
	const int screenHeight = 720;

	InitWindow(screenWidth, screenHeight, "3D Fun");
	SetMousePosition(screenWidth / 2, screenHeight / 2);

	Player player;
	Enemy enemy({1, 4, 20}, "assets/models/Joesama.obj", "assets/models/image0.png");
	Bow bow;
	

	Camera3D camera = { 0 };
	camera.position = {};
	camera.target = {};
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;
	// To prevent the y coordinate from being effected by movement


	// GUI Variables
	float score{};
	

	float mouseSensitivity = 0.003f;
	float yaw = 0.0f;
	float pitch = 0.0f;
	float moveSpeed = 0.15;
	float timeMouseHeld = 0.0f;
	const float maxTimeHeld = 2.f;
	float initialYawRadians;
	float arrowSize = 0.0f;



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
	bool debugEnabled = false;
	bool trackArrowBool = false;
	bool enemyIsHit = false;



	DisableCursor();

	SetTargetFPS(60);

	while (!WindowShouldClose()) {

		camera.position = { player.position };
		camera.target = { player.target };

		float deltaTime = GetFrameTime();

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
				(player.position.x + arrowDirX * arrowDistance),
				(player.position.y + arrowDirY * arrowDistance),
				(player.position.z + arrowDirZ * arrowDistance)
		};
		

		Vector3 forward = {
				cosf(pitch) * sinf(yaw),
				sinf(pitch),
				cosf(pitch) * cosf(yaw)
		};

		// Normalize movement
		forward = Vector3Normalize(forward);
		// Calculate the right vector
		Vector3 right = Vector3CrossProduct(forward, Vector3Normalize(camera.up));
		right.y = 0.0f;
		// normalize the right vector
		right = Vector3Normalize(right);
		camera.target = Vector3Add(player.position, forward);


		// Shoots a ray into the ground to predict jump landing
		Ray ray = { player.position, {0.0f, -1.0f, 0.0f} };
		Vector3 endPoint = Vector3Add(ray.position, Vector3Scale(ray.direction, 10.0f));
		rayDistY = (groundY - ray.position.y) / ray.direction.y;


		// ------------------ Camera Debug ----------------------
		//std::cout << "endPoint: " << endPoint.y << std::endl;
		//std::cout << "Camera PosY: " << camera.position.y << std::endl;
		//std::cout << "RayDistY: " << rayDistY << std::endl;
		//std::cout << "isOnGround: " << isOnGround << std::endl;
		//std::cout << "Jumping: " << jumping << std::endl;
		//std::cout << "Velocity: " << velocity << std::endl;
		//std::cout << "-----------------------" << std::endl;


	
		//enemy.chasePlayer(player.position, deltaTime);

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
			player.target.y += velocity;
			player.position.y += velocity;
			arrowPosition.y += velocity;
			velocity -= gravity;
		}

		// Normalizes the Y coordinate
		if (isOnGround && !jumping) {
			player.position.y = groundHeight;
			velocity = 0.0f;
			jumping = false;
		}


		if (IsKeyDown(KEY_LEFT_SHIFT)) {
			moveSpeed = 0.65f;
		}
		else {
			moveSpeed = 0.4f;
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
			std::cout << "------------------------------BOW FIRED-----------------------------" << std::endl;
			initialYawRadians = yaw;

			trackArrowBool = true;
			bow.printResults();

		}
		if (IsKeyPressed(KEY_P)) {
			debugEnabled = !debugEnabled;
		}		
		if (IsKeyDown(KEY_W)) {
			if (!jumping) {
				player.position = Vector3Add(player.position, Vector3Scale(forward, moveSpeed));
				player.position.y = groundHeight;
				player.target = Vector3Add(player.position, forward);
			}
		}
		if (IsKeyDown(KEY_S)) {
			if (!jumping) {
				player.position = Vector3Subtract(player.position, Vector3Scale(forward, moveSpeed));
				player.position.y = groundHeight;
				player.target = Vector3Add(player.position, forward);

			}
		}
		if (IsKeyDown(KEY_A)) {
			if (!jumping) {
				player.position = Vector3Subtract(player.position, Vector3Scale(right, moveSpeed));
				player.target = Vector3Subtract(player.target, Vector3Scale(right, moveSpeed));
			}
		}
		if (IsKeyDown(KEY_D)) {
			if (!jumping) {
				player.position = Vector3Add(player.position, Vector3Scale(right, moveSpeed));
				player.target = Vector3Add(player.target, Vector3Scale(right, moveSpeed));
			}
		}

		// ----------------Arrow Logic----------------------
		float arrowSpeed = 0.5f;

		arrowPosition.x += bow.arrowDest.x * arrowSpeed;
		arrowPosition.y += bow.arrowDest.y * arrowSpeed;
		arrowPosition.z += bow.arrowDest.z * arrowSpeed;
		float arrowRadius = 1.f;

		
		Vector3 arrowCollisionSphere = {
			arrowPosition.x,
			arrowPosition.y,
			arrowPosition.z
		};

		

		// This Resets the arrow position
		if (trackArrowBool) {
			arrowSize = .2f;
			bow.trackArrow(initialYawRadians);

			if (arrowPosition.y <= 0.f) {
				arrowSize = 0.0f;
				bow.arrowDest = { 0.0f, 0.0f, 0.0f };
				bow.time = 0.0f;
				enemy.enemyColor = WHITE;
				trackArrowBool = false;
				//std::cout << std::fixed << std::setprecision(2);
				//std::cout << "RSX: " << rayStart.x << " REX: " << rayEnd.x << "RSY: " << rayStart.y << " REY: " << rayEnd.y << "RSZ: " << rayStart.z << " REZ: " << rayEnd.z << std::endl;
			}
		}
		/*
		if (trackArrowBool) {
			camera.position = Vector3Lerp(camera.position, bow.arrowDest, 0.2f);
			camera.target = bow.arrowDest;
		}
		*/

		// Checks collisiong between arrow and enemy. Not perfect, maybe we need to cast a bigger sphere
		if (CheckCollisionSpheres(arrowCollisionSphere, arrowRadius, enemy.position, enemy.boundingSphereRadius)) {
			if (!enemyIsHit) {
				if (enemy.distanceToPlayer >= enemy.minDistance) {
					score += 1;
					enemy.enemyColor = RED;
					enemyIsHit = true;
					std::cout << "Arrow Hit: " << score << std::endl;
					std::cout << "ArrowColY: " << arrowCollisionSphere.y << std::endl;
					std::cout << "EnemyPosY: " << enemy.position.y << std::endl;
				}
			}
		}
		else {

			enemyIsHit = false;
		}

		
		//-------------------------------------------------

		BeginDrawing();
		ClearBackground(RAYWHITE);
		//Draw GUI here


		BeginMode3D(camera);
		// Draw 3D elements here
		 
		enemy.draw(player.position);

		// Jumping ray
		//DrawLine3D(ray.position, endPoint, RED);
		//DrawSphere(enemy.position, 4.f, BLUE);
		DrawSphere(arrowPosition, arrowSize, RED);
		DrawGrid(100, 10); 


		EndMode3D();
		DrawReticle(screenWidth, screenHeight, 7.5f);
		DrawRectangle(screenWidth / 2 + 150, (screenHeight / 2 + 100) - bow.displacement * 100.f, 25, (bow.displacement * 100.f), GREEN);

		DrawText(TextFormat("Score: %.2f", score), 10, 10, 20, BLACK);

	
		if (debugEnabled) {

			DrawText(TextFormat("Debug Enabled"), screenWidth / 2 - 25, 10, 20, BLACK);

			DrawText(TextFormat("Displacement: %.2f", bow.displacement), 10, 30, 20, BLACK);
			DrawText(TextFormat("Angle: %.2f", bow.angleDegrees), 10, 50, 20, BLACK);
			DrawText(TextFormat("Direction: %.2f", yaw), 10, 70, 20, BLACK);
			DrawText(TextFormat("Bow Elastic Potential: %.2f", bow.elasticPotential), 10, 90, 20, BLACK);
			DrawText(TextFormat("Arrow Velocity: %.2f", bow.velocity), 10, 110, 20, BLACK);
			DrawText(TextFormat("Force Required: %.2f", bow.forceRequired), 10, 130, 20, BLACK);
			DrawText(TextFormat("Distance Arrow Traveled: %.2f", bow.arrowDistance), 10, 150, 20, BLACK);


			DrawText(TextFormat("Time Step: %.2f seconds", bow.time), 10, 170, 20, BLACK);
			DrawText(TextFormat("Arrow X Position: %.2f meters", bow.arrowDest.x), 10, 190, 20, BLACK);
			DrawText(TextFormat("Arrow Y Position: %.2f meters", bow.arrowDest.y), 10, 210, 20, BLACK);
			DrawText(TextFormat("Arrow Distance: %.2f meters", bow.arrowDistance), 10, 230, 20, BLACK);

			DrawText(TextFormat("X: %.2f", player.position.x), screenWidth - 100, 10, 20, BLACK);
			DrawText(TextFormat("Y: %.2f", player.position.y), screenWidth - 100, 30, 20, BLACK);
			DrawText(TextFormat("Z: %.2f", player.position.z), screenWidth - 100, 50, 20, BLACK);

		}


		EndDrawing();

	}

	CloseWindow();

	return 0;
}
